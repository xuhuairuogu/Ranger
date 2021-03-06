#ifndef __GAITCONTROL_H__
#define __GAITCONTROL_H__


/* These are the key variables - they are used to compute the 
 * control actions used by the walking finite state machine */
extern float GAIT_WALK_ANK_PUSH;
extern float GAIT_WALK_CRIT_STEP_LENGTH;
extern float GAIT_WALK_SCISSOR_GAIN;
extern float GAIT_WALK_SCISSOR_OFFSET;
// extern float GAIT_WALK_PUSH_INTEGRAL;
extern float GAIT_WALK_DS_DELAY;

/* Entry-point functions */
void gaitControl_entry(void);
void gaitControl_main(void);

#endif // __GAITCONTROL_H__
