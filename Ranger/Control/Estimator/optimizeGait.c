#include <mb_includes.h>
#include <mb_estimator.h>
#include <mb_controller.h>
#include <input_output.h>
#include <optimizeGait.h>
#include <walkControl.h>
#include <gaitControl.h>
#include <gaitControlData.h>
#include "RangerMath.h"
#include "PSO.h"

/* MECHANICS OF OPTIMIZATION TRIALS
 *
 * Before starting a new trial, press the Accept button once to start optimization.
 *
 * Whenever the robot is walking it is quietly evaluating the gait in the background.
 * A trial begins when the robot transitions from flight phase (in walk mode) to single
 * stance, since this is how walking is initiated. The objective function then logs
 * data for a preset number of steps, and then ignores all subsequent steps. A trial
 * is completed when the robot either falls down or is picked up. At this point, the
 * user has a choice. If he/she does nothing, then the objective function will be
 * cleared when the robot shuts down, or starts walking again. On the other hand, if the
 * user presses the ACCEPT button (currently button 0), then the optimization will log
 * the trial, sending the data to the optimization algorithm. The optimization will then
 * sample a new point, and then update the controller parameters to match this new point.
 * This way, when a new trial starts it will be evaluating the next point in the search.
 *
 * When the robot transitions back to "standby" mode, the current set of walk parameters
 * are returned to default, but the optimization data remains, and returning to a walk
 * mode will continue the optimization where it left off.
 *
 */

#define N_DEC_VARS 5   // Number of decision variables used in the optimization

static int STEP_COUNT = 0;
static const int N_STEP_TRANSIENT = 5;  // Ignore the first few steps to reject transients

static const int N_STEP_TRIAL = 12; // Include this many steps in objective function
static const int N_POPULATION = 6; // population to use in optimization

static const float OMEGA = 0.5;   // PSO inertial weighting term
static const float ALPHA = 0.9;   // PSO local search weight
static const float BETA = 0.9;    // PSO global search weight

static float STEP_DURATION_TARGET = 0.62;  // Target duration for each walking step, if used

static float STEP_LENGTH[N_STEP_TRIAL];
static float STEP_DURATION[N_STEP_TRIAL];

static const float MIN_STEP_LENGTH = 0.1;
static float lastStepLength = 0.0;

static float lastTrialStartTime = 0.0; // Keep track of the time at which a new trial starts. Send this many times to make sure we have a correct starting timestamp.

float X_DEFAULT[N_DEC_VARS];   // Default control, send as hint;


OptimizeFsmMode OPTIMIZE_FSM_MODE = INIT;



/*******************************************************************************
 *                    PRIVATE UTILITY FUNCTIONS                                *
 *******************************************************************************/


/* Computes the mean of the squared error in speed
 * mean(((d/t)-v0).^2); */
float getMeanSpeedSquaredError(void) {
	float errorSquared[N_STEP_TRIAL];
	float err;					 
	int stepCountIdx;  // counter
	for (stepCountIdx = 0;  stepCountIdx < N_STEP_TRIAL; stepCountIdx++) {
		err = (STEP_LENGTH[stepCountIdx]/STEP_DURATION[stepCountIdx]) - GAITDATA_TARGET_SPEED;
		errorSquared[stepCountIdx] = err * err;
	}
	return Mean(errorSquared,N_STEP_TRIAL);
}



/* Computes the mean of the squared error in step duration
 * mean((t-t0).^2); */
float getMeanDurationSquaredError(void) {
	float errorSquared[N_STEP_TRIAL];
	float err;					 
	int stepCountIdx;  // counter
	for (stepCountIdx = 0;  stepCountIdx < N_STEP_TRIAL; stepCountIdx++) {
		err = STEP_DURATION[stepCountIdx] - STEP_DURATION_TARGET;
		errorSquared[stepCountIdx] = err * err;
	}
	return Mean(errorSquared,N_STEP_TRIAL);
}

/* Computes the error in average speed over the entire trial, squared.
 * (sum(d) / sum(t))^2; */
float getAverageSpeedError(void) {
	float speedError = 0.0;
	float totalStepDuration = 0.0;
	float totalStepLength = 0.0;
	int stepCountIdx;  // counter
	for (stepCountIdx = 0;  stepCountIdx < N_STEP_TRIAL; stepCountIdx++) {
		totalStepDuration += STEP_DURATION[stepCountIdx];
		totalStepLength += STEP_LENGTH[stepCountIdx];
	}
	speedError = GAITDATA_TARGET_SPEED - (totalStepLength / totalStepDuration);
	return speedError * speedError;
}


/* This function is called at the start of a new trial. This occurs whenever the
 * robot transitions from flight phase to single stance.  */
void resetObjective(void) {
	int stepCountIdx;  // counter
	STEP_COUNT = -N_STEP_TRANSIENT;

	for (stepCountIdx = 0;  stepCountIdx < N_STEP_TRIAL; stepCountIdx++) {
		STEP_DURATION[stepCountIdx] = 2.0;   //  Default to a step duration of 2 seconds
		STEP_LENGTH[stepCountIdx] = 0.0;  // Default to zero step length
	}
}



/*******************************************************************************
 *                      PSO INTERFACE FUNCTIONS                                *
 *******************************************************************************/


/* Objective function for the optimization. */
void objFun_send(float* x, int nDim) {
	/// Update the middle value on each data channel. Assume that the other values [0] and [2] are good enough
	GAITDATA_WALK_SCISSOR_OFFSET[1] = x[0];
	GAITDATA_WALK_SCISSOR_GAIN[1] = x[1];
	GAITDATA_WALK_ANK_PUSH[1] = x[2];
	GAITDATA_WALK_CRIT_STEP_LENGTH[1] = x[3];
	// GAITDATA_WALK_PUSH_INTEGRAL[1] = x[4];
	GAITDATA_WALK_DS_DELAY[1] = x[4];

	/// Send out over CAN network
	mb_io_set_float(ID_OPTIM_WALK_SCISSOR_OFFSET, x[0]);
	mb_io_set_float(ID_OPTIM_WALK_SCISSOR_GAIN, x[1]);
	mb_io_set_float(ID_OPTIM_WALK_ANK_PUSH, x[2]);
	mb_io_set_float(ID_OPTIM_WALK_CRIT_STEP_LENGTH, x[3]);
	// mb_io_set_float(ID_OPTIM_WALK_PUSH_INTEGRAL, x[4]);
	mb_io_set_float(ID_OPTIM_WALK_DS_DELAY, x[4]);
}


/* Returns the value of the objective function at the last called
 * query point. Accumulates the cost function. */
float objFun_eval(void) {
	float objVal;
	// objVal = getAverageSpeedError();   // (sum(d) / sum(t))^2;
	// objVal = getMeanDurationSquaredError();  // mean((t-t0).^2);
	objVal = getMeanSpeedSquaredError();  // mean(((d/t)-v0).^2); 
	mb_io_set_float(ID_OPTIM_OBJ_FUN_LAST_VAL, objVal); // Report to LabView.
	return objVal;
}


/* This function should be called once to create the optimization
 * problem, and then pass this information along to the optimization
 * method. */
void objFun_set_optimizeGait(void) {

	int dim;
	int nDim = N_DEC_VARS;
	int nPop = N_POPULATION;
	void (*objFunSend)(float * x, int nDim);
	float (*objFunEval)();
	float xLow[N_DEC_VARS];
	float xUpp[N_DEC_VARS];
	objFunSend = &objFun_send;
	objFunEval = &objFun_eval;
	for (dim = 0; dim < nDim; dim++) {
		xLow[dim] = GAITDATA_LOW_BOUND[dim];
		xUpp[dim] = GAITDATA_UPP_BOUND[dim];
	}

	PSO_OMEGA = OMEGA;
	PSO_ALPHA = ALPHA;
	PSO_BETA = BETA;
	setObjFunInfo(xLow, xUpp, nDim, nPop, objFunSend, objFunEval); // Send problem to PSO

	/// Copy the default gait data for using later as a hint.
	X_DEFAULT[0] = GAITDATA_WALK_SCISSOR_OFFSET[1];
	X_DEFAULT[1] = GAITDATA_WALK_SCISSOR_GAIN[1];
	X_DEFAULT[2] = GAITDATA_WALK_ANK_PUSH[1];
	X_DEFAULT[3] = GAITDATA_WALK_CRIT_STEP_LENGTH[1];
	// X_DEFAULT[4] = GAITDATA_WALK_PUSH_INTEGRAL[1];
	X_DEFAULT[4] = GAITDATA_WALK_DS_DELAY[1];

}



/*******************************************************************************
 *                         FINITE STATE MACHINE                                *
 *******************************************************************************/

void updateOptimizeFsm(void) {

	bool accept = detect_UI_button_input(BUTTON_ACCEPT_TRIAL);
	bool reject = detect_UI_button_input(BUTTON_REJECT_TRIAL);

	bool flagAccept = false;
	bool flagReject = false;

	switch (OPTIMIZE_FSM_MODE) {

	case INIT:
		if (accept) {
			objFun_set_optimizeGait();    // Set up optimization problem

			lastTrialStartTime = mb_io_get_float(ID_TIMESTAMP); // Record the start time of this new trial.
			mb_io_set_float(ID_OPTIM_LAST_START_TIME, lastTrialStartTime);

			psoGiveHint(X_DEFAULT);  // Give PSO a good place to start

			pso_send_point();   // Tell PSO to send a new point (Updates controller)
			OPTIMIZE_FSM_MODE = PRE_1;
		}
		break;


	case PRE_1:
		if (STATE_contactMode == CONTACT_FL) {
			OPTIMIZE_FSM_MODE = PRE_2;
		}
		break;


	case PRE_2:
		if (STATE_contactMode != CONTACT_FL) {
			resetObjective();	// Clears objective and sets up for new trial
			OPTIMIZE_FSM_MODE = TRANS;
		}
		break;


	case TRANS:
		if (STEP_COUNT >= 0) {
			OPTIMIZE_FSM_MODE = TRIAL;
		}
		if (accept) {
			flagAccept = true;
			OPTIMIZE_FSM_MODE = PRE_1;
		} else if (reject) {
			flagReject = true;
			OPTIMIZE_FSM_MODE = PRE_1;
		}
		break;


	case TRIAL:
		if (STEP_COUNT >= N_STEP_TRIAL) {
			OPTIMIZE_FSM_MODE = POST;
		}
		if (accept) {
			flagAccept = true;
			OPTIMIZE_FSM_MODE = PRE_1;
		} else if (reject) {
			flagReject = true;
			OPTIMIZE_FSM_MODE = PRE_1;
		}
		break;


	case POST:
		if (lastStepLength > MIN_STEP_LENGTH  && STATE_contactMode != CONTACT_FL) {
			flagAccept = true;
			OPTIMIZE_FSM_MODE = PRE_2;
		} else {
			if (accept) {
				flagAccept = true;
				OPTIMIZE_FSM_MODE = PRE_1;
			} else if (reject) {
				flagReject = true;
				OPTIMIZE_FSM_MODE = PRE_1;
			}
		}
		break;
	}

	if (flagAccept) {

		lastTrialStartTime = mb_io_get_float(ID_TIMESTAMP); // Record the start time of this new trial.
		mb_io_set_float(ID_OPTIM_LAST_START_TIME, lastTrialStartTime);

		pso_eval_point();   // Evaluate objective function (send to PSO)
		pso_send_point();   // Tell PSO to send a new point (Updates controller)
		mb_io_set_float(ID_OPTIM_BUTTON_PUSH, 1.0);
	} else if (flagReject) {

		lastTrialStartTime = mb_io_get_float(ID_TIMESTAMP); // Record the start time of this new trial.
		mb_io_set_float(ID_OPTIM_LAST_START_TIME, lastTrialStartTime);

		mb_io_set_float(ID_OPTIM_BUTTON_PUSH, 0.0);
	}
	flagAccept = false;
	flagReject = false;

	// Keep sending the timestamp of the most recent trial's beginning.
	mb_io_set_float(ID_OPTIM_LAST_START_TIME, lastTrialStartTime);
}


/* Sets the color for the LED indicator on the board. */
void updateOptimizeLed(void) {
	switch (OPTIMIZE_FSM_MODE) {
	case PRE_1:
		set_UI_LED(LED_OPTIMIZE, 'o');
		break;

	case PRE_2:
		set_UI_LED(LED_OPTIMIZE, 'g');
		break;

	case TRANS:
		set_UI_LED(LED_OPTIMIZE, 'r');
		break;

	case TRIAL:
		set_UI_LED(LED_OPTIMIZE, 'y');
		break;

	case POST:
		set_UI_LED(LED_OPTIMIZE, 'b');
		break;
	}

}




/*******************************************************************************
 *                    PUBLIC INTERFACE FUNCTIONS                               *
 *******************************************************************************/

/* This function is called by the estimator each time that a step occurs */
void logStepData(double duration, double length) {
	float speed = 0;
	if (duration != 0) {  // No divide by zero errors!  (keep default=0 if so)
		speed = length / duration;  // Log the speed data
	}
	if (OPTIMIZE_FSM_MODE == TRIAL) {
		STEP_DURATION[STEP_COUNT] = duration;
		STEP_LENGTH[STEP_COUNT] = length;
	}

	STEP_COUNT++;   // THIS COMES AFTER objFun_runningAvg()  <-- important !
	lastStepLength = length;

	mb_io_set_float(ID_OPTIM_STEP_COUNT, STEP_COUNT);
	mb_io_set_float(ID_OPTIM_OBJ_FUN_SPEED, speed);
	mb_io_set_float(ID_OPTIM_OBJ_FUN_LENGTH, length);
}


/* Call once per tick during walking. Checks for the start of a new walking trial. */
void optimizeGait_main(void) {
	updateOptimizeFsm();
	updateOptimizeLed();
}
