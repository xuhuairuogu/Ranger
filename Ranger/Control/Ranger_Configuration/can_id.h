//
//CAN_ID Table for Cornell Ranger Robot
//Automatically generated by ranger_parser.m
//	 use the ranger_parser.m MATLAB script to make any changes to can_id.h, board_id.h, error_id.h, init_txlist.h, init_rxlist.h, io_data.c, error_messages.h or csr_routing_table.c
//	 the script is located in ...\SVN\Trunk\Ranger\Control\Ranger_Configuration
//

#ifndef __H_CAN_ID__
#define __H_CAN_ID__ 

typedef enum can_ids{
	ID_TIMESTAMP = 0,	 // 0 2 1 // Timestamp
	ID_ERROR_MB = 1,	 // 0 1 0 // 
	ID_ERROR_CSR = 2,	 // 0 2 1 // 
	ID_ERROR_MCH = 3,	 // 0 3 1 // 
	ID_ERROR_MCFO = 4,	 // 0 4 1 // 
	ID_ERROR_MCFI = 5,	 // 0 5 1 // 
	ID_ERROR_MCSO = 6,	 // 0 6 1 // 
	ID_ERROR_MCSI = 7,	 // 0 7 1 // 
	ID_ERROR_UI = 8,	 // 0 8 1 // 
	ID_MB_STATUS = 9,	 // 0 1 3;4;5;6;7;8;9 // 
	ID_MCH_MOTOR_VELOCITY = 10,	 // 0 3 1 // 
	ID_MCH_MOTOR_CURRENT = 11,	 // 0 3 1 // 
	ID_MCH_MOTOR_POSITION = 12,	 // 0 3 1 // 
	ID_MCH_MOTOR_TARGET_CURRENT = 13,	 // 0 1 3 // 
	ID_MCH_ANGLE = 14,	 // 0 3 1 // 
	ID_MCH_BATT_POWER = 15,	 // 0 3 1 // 
	ID_MCH_SHUTDOWN = 16,	 // 0 1 3 // 
	ID_MCH_SLEEP = 17,	 // 0 1 3 // 
	ID_MCH_COMMAND_CURRENT = 18,	 // 0 1 3 // 
	ID_MCH_STIFFNESS = 19,	 // 0 1 3 // 
	ID_MCH_DAMPNESS = 20,	 // 0 1 3 // 
	ID_MCH_ANG_RATE = 21,	 // 0 3 1 // 
	ID_MCH_STATUS = 22,	 // 0 3 1 // 
	ID_MCH_EXECUTION_TIME = 23,	 // 0 3 1 // 
	ID_MCH_MAX_EXECUTION_TIME = 24,	 // 0 3 1 // 
	ID_MCH_EMPTY_TX1 = 25,	 // 0 1 3 // 
	ID_MCH_EMPTY_TX2 = 26,	 // 0 1 3 // 
	ID_MCH_BATT_CURRENT = 27,	 // 0 3 1 // 
	ID_MCH_BATT_VOLTAGE = 28,	 // 0 3 1 // 
	ID_MCH_HBRIDGE_TEMP = 29,	 // 0 3 1 // 
	ID_MCH_EMPTY_RX1 = 30,	 // 0 3 1 // 
	ID_MCH_EMPTY_RX2 = 31,	 // 0 3 1 // 
	ID_MCH_EMPTY_RX3 = 32,	 // 0 3 1 // 
	ID_MCFO_MOTOR_POSITION = 33,	 // 0 4 1 // 
	ID_MCFO_MOTOR_VELOCITY = 34,	 // 0 4 1 // 
	ID_MCFO_MOTOR_CURRENT = 35,	 // 0 4 1 // 
	ID_MCFO_MOTOR_TARGET_CURRENT = 36,	 // 0 1 4 // 
	ID_MCFO_RIGHT_ANKLE_ANGLE = 37,	 // 0 4 1 // 
	ID_MCFO_RIGHT_LS = 38,	 // 0 4 1 // 
	ID_MCFO_LEFT_LS = 39,	 // 0 4 1 // 
	ID_MCFO_RIGHT_HS = 40,	 // 0 4 1 // 
	ID_MCFO_LEFT_HS = 41,	 // 0 4 1 // 
	ID_MCFO_SHUTDOWN = 42,	 // 0 1 4 // 
	ID_MCFO_SLEEP = 43,	 // 0 1 4 // 
	ID_MCFO_BATT_POWER = 44,	 // 0 4 1 // 
	ID_MCFO_COMMAND_CURRENT = 45,	 // 0 1 4 // 
	ID_MCFO_STIFFNESS = 46,	 // 0 1 4 // 
	ID_MCFO_DAMPNESS = 47,	 // 0 1 4 // 
	ID_MCFO_STATUS = 48,	 // 0 4 1 // 
	ID_MCFO_LEFT_HEEL_SENSE = 49,	 // 0 4 1 // 
	ID_MCFO_RIGHT_HEEL_SENSE = 50,	 // 0 4 1 // 
	ID_MCFO_EXECUTION_TIME = 51,	 // 0 4 1 // 
	ID_MCFO_MAX_EXECUTION_TIME = 52,	 // 0 4 1 // 
	ID_MCFO_HS_STATE = 53,	 // 0 1 4 // 
	ID_MCFO_EMPTY_TX2 = 54,	 // 0 1 4 // 
	ID_MCFO_BATT_CURRENT = 55,	 // 0 4 1 // 
	ID_MCFO_BATT_VOLTAGE = 56,	 // 0 4 1 // 
	ID_MCFO_HBRIDGE_TEMP = 57,	 // 0 4 1 // 
	ID_MCFO_OUTER_HS = 58,	 // 0 4 1 // 
	ID_MCFO_RIGHT_ANKLE_RATE = 59,	 // 0 4 1 // 
	ID_MCFO_EMPTY_RX3 = 60,	 // 0 4 1 // 
	ID_MCFI_MOTOR_VELOCITY = 61,	 // 0 5 1 // 
	ID_MCFI_MOTOR_CURRENT = 62,	 // 0 5 1 // 
	ID_MCFI_MOTOR_POSITION = 63,	 // 0 5 1 // 
	ID_MCFI_MOTOR_TARGET_CURRENT = 64,	 // 0 1 5 // 
	ID_MCFI_MID_ANKLE_ANGLE = 65,	 // 0 5 1 // 
	ID_MCFI_RIGHT_LS = 66,	 // 0 5 1 // 
	ID_MCFI_LEFT_LS = 67,	 // 0 5 1 // 
	ID_MCFI_RIGHT_HS = 68,	 // 0 5 1 // 
	ID_MCFI_LEFT_HS = 69,	 // 0 5 1 // 
	ID_MCFI_BATT_POWER = 70,	 // 0 5 1 // 
	ID_MCFI_SHUTDOWN = 71,	 // 0 1 5 // 
	ID_MCFI_SLEEP = 72,	 // 0 1 5 // 
	ID_MCFI_COMMAND_CURRENT = 73,	 // 0 1 5 // 
	ID_MCFI_STIFFNESS = 74,	 // 0 1 5 // 
	ID_MCFI_DAMPNESS = 75,	 // 0 1 5 // 
	ID_MCFI_STATUS = 76,	 // 0 5 1 // 
	ID_MCFI_LEFT_HEEL_SENSE = 77,	 // 0 5 1 // 
	ID_MCFI_RIGHT_HEEL_SENSE = 78,	 // 0 5 1 // 
	ID_MCFI_EXECUTION_TIME = 79,	 // 0 5 1 // 
	ID_MCFI_MAX_EXECUTION_TIME = 80,	 // 0 5 1 // 
	ID_MCFI_HS_STATE = 81,	 // 0 1 5 // 
	ID_MCFI_EMPTY_TX2 = 82,	 // 0 1 5 // 
	ID_MCFI_BATT_CURRENT = 83,	 // 0 5 1 // 
	ID_MCFI_BATT_VOLTAGE = 84,	 // 0 5 1 // 
	ID_MCFI_HBRIDGE_TEMP = 85,	 // 0 5 1 // 
	ID_MCFI_INNER_HS = 86,	 // 0 5 1 // 
	ID_MCFI_ANKLE_RATE = 87,	 // 0 5 1 // 
	ID_MCFI_EMPTY_RX3 = 88,	 // 0 5 1 // 
	ID_COL_STATUS = 89,	 // 0 9 1 // 
	ID_COL_FRONT_WHITE = 90,	 // 0 9 1 // 
	ID_COL_FRONT_RED = 91,	 // 0 9 1 // 
	ID_COL_FRONT_GREEN = 92,	 // 0 9 1 // 
	ID_COL_FRONT_BLUE = 93,	 // 0 9 1 // 
	ID_COL_EXECUTION_TIME = 94,	 // 0 9 1 // 
	ID_COL_MAX_EXECUTION_TIME = 95,	 // 0 9 1 // 
	ID_ERROR_COL = 96,	 // 0 9 1 // 
	ID_COL_EMPTY2 = 97,	 // 0 9 1 // 
	ID_COL_EMPTY3 = 98,	 // 0 9 1 // 
	ID_MCSO_LEFT_ANKLE_ANGLE = 99,	 // 0 6 1;4 // 
	ID_MCSO_LEFT_ANKLE_RATE = 100,	 // 0 6 1 // 
	ID_MCSO_EMPTY_TX1 = 101,	 // 0 1 6 // 
	ID_MCSO_EMPTY_TX2 = 102,	 // 0 1 6 // 
	ID_MCSO_STATUS = 103,	 // 0 6 1 // 
	ID_MCSO_COLOR_BACK_WHITE = 104,	 // 0 6 1 // 
	ID_MCSO_COLOR_BACK_RED = 105,	 // 0 6 1 // 
	ID_MCSO_COLOR_BACK_GREEN = 106,	 // 0 6 1 // 
	ID_MCSO_COLOR_BACK_BLUE = 107,	 // 0 6 1 // 
	ID_MCSO_EXECUTION_TIME = 108,	 // 0 6 1 // 
	ID_MCSO_MAX_EXECUTION_TIME = 109,	 // 0 6 1 // 
	ID_MCSI_MOTOR_CURRENT = 110,	 // 0 7 1 // 
	ID_MCSI_MOTOR_TARGET_CURRENT = 111,	 // 0 1 7 // 
	ID_MCSI_STEER_ANGLE = 112,	 // 0 7 1 // 
	ID_MCSI_BATT_POWER = 113,	 // 0 7 1 // 
	ID_MCSI_SHUTDOWN = 114,	 // 0 1 7 // 
	ID_MCSI_SLEEP = 115,	 // 0 1 7 // 
	ID_MCSI_COMMAND_CURRENT = 116,	 // 0 1 7 // 
	ID_MCSI_STIFFNESS = 117,	 // 0 1 7 // 
	ID_MCSI_DAMPNESS = 118,	 // 0 1 7 // 
	ID_MCSI_STATUS = 119,	 // 0 7 1 // 
	ID_MCSI_EXECUTION_TIME = 120,	 // 0 7 1 // 
	ID_MCSI_MAX_EXECUTION_TIME = 121,	 // 0 7 1 // 
	ID_MCSI_EMPTY_TX1 = 122,	 // 0 1 7 // 
	ID_MCSI_EMPTY_TX2 = 123,	 // 0 1 7 // 
	ID_MCSI_BATT_CURRENT = 124,	 // 0 7 1 // 
	ID_MCSI_BATT_VOLTAGE = 125,	 // 0 7 1 // 
	ID_MCSI_HBRIDGE_TEMP = 126,	 // 0 7 1 // 
	ID_MCSI_EMPTY_RX1 = 127,	 // 0 7 1 // 
	ID_MCSI_EMPTY_RX2 = 128,	 // 0 7 1 // 
	ID_MCSI_EMPTY_RX3 = 129,	 // 0 7 1 // 
	ID_UI_SET_LCD_QUAD_1 = 130,	 // 0 1 8 // 
	ID_UI_SET_LCD_QUAD_2 = 131,	 // 0 1 8 // 
	ID_UI_SET_LCD_QUAD_3 = 132,	 // 0 1 8 // 
	ID_UI_SET_LCD_QUAD_4 = 133,	 // 0 1 8 // 
	ID_UI_SET_LED_1 = 134,	 // 0 1 8 // 
	ID_UI_SET_LED_2 = 135,	 // 0 1 8 // 
	ID_UI_SET_LED_3 = 136,	 // 0 1 8 // 
	ID_UI_SET_LED_4 = 137,	 // 0 1 8 // 
	ID_UI_SET_LED_5 = 138,	 // 0 1 8 // 
	ID_UI_SET_LED_6 = 139,	 // 0 1 8 // 
	ID_UI_SET_BUZZER_FREQ = 140,	 // 0 1 8 // 
	ID_UI_SET_BUZZER_AMPL = 141,	 // 0 1 8 // 
	ID_UI_ROLL = 142,	 // 0 8 1 // 
	ID_UI_PITCH = 143,	 // 0 8 1 // 
	ID_UI_YAW = 144,	 // 0 8 1 // 
	ID_UI_ANG_RATE_X = 145,	 // 0 8 1 // 
	ID_UI_ANG_RATE_Y = 146,	 // 0 8 1 // 
	ID_UI_ANG_RATE_Z = 147,	 // 0 8 1 // 
	ID_UI_BUTTONS = 148,	 // 0 8 1 // 
	ID_UI_RC_0 = 149,	 // 0 8 1 // 
	ID_UI_RC_1 = 150,	 // 0 8 1 // 
	ID_UI_RC_2 = 151,	 // 0 8 1 // 
	ID_UI_RC_3 = 152,	 // 0 8 1 // 
	ID_UI_EXECUTION_TIME = 153,	 // 0 8 1 // 
	ID_UI_MAX_EXECUTION_TIME = 154,	 // 0 8 1 // 
	ID_UI_AUDIO = 155,	 // 0 1 8 // 
	ID_UI_EMPTY_TX2 = 156,	 // 0 1 8 // 
	ID_UI_STATUS = 157,	 // 0 8 1 // 
	ID_UI_EMPTY_RX4 = 158,	 // 0 8 1 // 
	ID_UI_EMPTY_RX5 = 159,	 // 0 8 1 // 
	ID_CSR_CAN1_LOAD = 160,	 // 0 2 0 // 
	ID_CSR_CAN2_LOAD = 161,	 // 0 2 0 // 
	ID_CSR_CAN3_LOAD = 162,	 // 0 2 0 // 
	ID_CSR_CAN4_LOAD = 163,	 // 0 2 0 // 
	ID_CSR_MCU_POWER = 164,	 // 0 2 0 // 
	ID_CSR_EMPTY_TX1 = 165,	 // 0 1 2 // 
	ID_CSR_EMPTY_TX2 = 166,	 // 0 1 2 // 
	ID_CSR_MCU_VOLTAGE = 167,	 // 0 2 1 // 
	ID_CSR_MCU_CURRENT = 168,	 // 0 2 1 // 
	ID_CSR_EMPTY_RX4 = 169,	 // 0 2 1 // 
	ID_CSR_EMPTY_RX5 = 170,	 // 0 2 1 // 
	ID_MB_EXECUTION_TIME = 171,	 // 0 1 0 // Time required to finish the schedule
	ID_ERROR_LABVIEW = 172,	 // 0 1 0 // Buffered errors for transmission to LabView/PC
	ID_ERROR_LCD = 173,	 // 0 1 8 // Buffered errors for transmission to Ranger LCD
	ID_E_MCH_ANG_RATE = 174,	 // 0 1 0 // estimated rate of the hip (STATE = dqh)
	ID_E_MCFO_RIGHT_ANKLE_RATE  = 175,	 // 0 1 0 // estimated rate of the outer ankles (STATE=dq0)
	ID_E_MCFI_ANKLE_RATE = 176,	 // 0 1 0 // estimated rate of the inner ankles (STATE = dq1)
	ID_EST_CONTACT_OUTER = 177,	 // 0 1 0 // Filtered sum of both outer foot contact sensors
	ID_EST_CONTACT_INNER = 178,	 // 0 1 0 // Filtered sum of both inner foot contact sensors
	ID_EST_STATE_CONTACTMODE = 179,	 // 0 1 0 // 0 = S0    1 = S1    2 = DS      3 = FL
	ID_EST_UI_FSM_MODE = 180,	 // 0 1 0 // 0 = Unit test     1 = WalkCtrl      2 = StandBy
	ID_EST_STATE_TH0 = 181,	 // 0 1 0 // abs angle outer legs
	ID_EST_STATE_TH1 = 182,	 // 0 1 0 // abs angle inner legs
	ID_EST_STATE_PHI0 = 183,	 // 0 1 0 // abs angle outer feet
	ID_EST_STATE_PHI1 = 184,	 // 0 1 0 // abs angle inner feet
	ID_EST_STATE_DTH0 = 185,	 // 0 1 0 // abs rate outer legs
	ID_EST_STATE_DTH1 = 186,	 // 0 1 0 // abs rate inner legs
	ID_EST_STATE_DPHI0 = 187,	 // 0 1 0 // abs rate outer feet
	ID_EST_STATE_DPHI1 = 188,	 // 0 1 0 // abs rate inner feet
	ID_EST_STATE_PSI = 189,	 // 0 1 0 // Steering angle  in radians
	ID_EST_STATE_TH0_GYRO = 190,	 // 0 1 0 // outer leg angle based on integral of rate gyro
	ID_EST_STATE_TH0_IMU = 191,	 // 0 1 0 // outer leg angle based on IMU sensor fusion 
	ID_EST_STATE_POSCOM = 192,	 // 0 1 0 // borizontal component of the center of mass position
	ID_EST_STATE_VELCOM = 193,	 // 0 1 0 // horizontal component of the center of mass velocity
	ID_EST_LAST_STEP_LENGTH = 194,	 // 0 1 0 // step length that was computed at the previous heel-strike
	ID_EST_LAST_STEP_ANGLE = 195,	 // 0 1 0 // the absolute angle of the outer leg that was computed at the previous heel-strike
	ID_EST_LAST_STEP_DURATION = 196,	 // 0 1 0 // Duration of the previous walking step.
	ID_EST_TOTAL_BATT_POWER = 197,	 // 0 1 0 // add up the battery power being consumed by all boards
	ID_OPTIM_WALK_ANK_PUSH  = 198,	 // 0 1 0 // magnitude of the push-off during walking  normalized to be on the range 0 to 1
	ID_OPTIM_WALK_CRIT_STANCE_ANGLE = 199,	 // 0 1 0 // the critical stance angle when push-off should occur (should be less than zero)
	ID_OPTIM_WALK_HIP_STEP_ANGLE  = 200,	 // 0 1 0 // Target angle for the hip to hold during push-off
	ID_OPTIM_WALK_SCISSOR_GAIN = 201,	 // 0 1 0 // 
	ID_OPTIM_WALK_SCISSOR_OFFSET = 202,	 // 0 1 0 // 
	ID_OPTIM_WALK_DS_DELAY = 203,	 // 0 1 0 // 
	ID_OPTIM_STEP_COUNT = 204,	 // 0 1 0 // Steps in the current optimization walk. Transient steps are negative.
	ID_OPTIM_ACTIVE_PARTICLE = 205,	 // 0 1 0 // PSO population particle active
	ID_OPTIM_CURRENT_GENERATION = 206,	 // 0 1 0 // 
	ID_OPTIM_OBJ_FUN_SPEED = 207,	 // 0 1 0 // average speed over the the last step
	ID_OPTIM_OBJ_FUN_LENGTH  = 208,	 // 0 1 0 // last step length
	ID_OPTIM_OBJ_FUN_LAST_VAL = 209,	 // 0 1 0 // last value of the objective function
	ID_OPTIM_FSM_MODE = 210,	 // 0 1 0 // 0 = INIT    1 = PRE_TRIAL    2 = TRIAL    3 = FLYING   
	ID_OPTIM_BUTTON_PUSH = 211,	 // 0 1 0 // 0 == reject      1 = accept
	ID_OPTIM_SAVE_OPTIM_STATE = 212,	 // 0 1 0 // Save an optimization so it can be loaded later. Order of parameter dump TBD
	ID_OPTIM_LAST_START_TIME = 213,	 // 0 1 0 // The time that the most recent trial has begun. Same value sent very frequently to make sure it gets through.
	ID_EST_ENERGY_MCH = 214,	 // 0 1 0 // Accumulated work since robot turned on - measured at board power supply.
	ID_EST_ENERGY_MCFO = 215,	 // 0 1 0 // Accumulated work since robot turned on - measured at board power supply.
	ID_EST_ENERGY_MCFI = 216,	 // 0 1 0 // Accumulated work since robot turned on - measured at board power supply.
	ID_EST_ENERGY_OVERHEAD = 217,	 // 0 1 0 // Accumulated work since robot turned on - measured at board power supply.
	ID_CTRL_HIP_KP = 218,	 // 25 0 1 // 
	ID_CTRL_HIP_KD = 219,	 // 3 0 1 // 
	ID_CTRL_ANK_PUSH_KP = 220,	 // 20 0 1 // 
	ID_CTRL_ANK_PUSH_KD = 221,	 // 2 0 1 // 
	ID_CTRL_ANK_STANCE_KP = 222,	 // 15 0 1 // 
	ID_CTRL_ANK_STANCE_KD = 223,	 // 1 0 1 // 
	ID_CTRL_ANK_SWING_KP = 224,	 // 20 0 1 // 
	ID_CTRL_ANK_SWING_KD = 225,	 // 1 0 1 // 
	ID_CTRL_HIP_COMPENSATION_TARGET = 226,	 // 0 0 1 // hip compensation at target (1) or at measured state (0)
	ID_CTRL_HIP_GRAVITY_COMPENSATION = 227,	 // 1 0 1 // Enables (1) or disables (0) gravity compensation in hip controller
	ID_CTRL_HIP_SPRING_COMPENSATION = 228,	 // 1 0 1 // Enables (1) or disables (0) spring compensation in hip controller
	ID_CTRL_WALK_ANK_PUSH  = 229,	 // 0.8 0 1 // magnitude of the push-off during walking  normalized to be on the range 0 to 1
	ID_CTRL_WALK_CRIT_STANCE_ANGLE = 230,	 // -0.08 0 1 // the critical stance angle when push-off should occur (should be less than zero)
	ID_CTRL_WALK_HIP_STEP_ANGLE  = 231,	 // 0.2 0 1 // Target angle for the hip to hold during push-off
	ID_CTRL_WALK_SCISSOR_GAIN = 232,	 // 1.2 0 1 // 
	ID_CTRL_WALK_SCISSOR_OFFSET = 233,	 // 0.1 0 1 // 
	ID_CTRL_WALK_DS_DELAY = 234,	 // 0.05 0 1 // How long to sustain push-off after heel-strike
	ID_CTRL_FSM_LED = 235,	 // 0 0 1 // 0 = LED OFF        1 = LED ON
	ID_CTRL_UNIT_TEST_ID = 236,	 // 0 0 1 // Selects which unit test the robot should be running. Default is zero = no test
	ID_CTRL_TEST_R0 = 237,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R1 = 238,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R2 = 239,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R3 = 240,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R4 = 241,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R5 = 242,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R6 = 243,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R7 = 244,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R8 = 245,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_R9 = 246,	 // 0 0 1 // test read from labview
	ID_CTRL_TEST_W0 = 247,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W1 = 248,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W2 = 249,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W3 = 250,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W4 = 251,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W5 = 252,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W6 = 253,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W7 = 254,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W8 = 255,	 // 0 1 0 // test write to labview
	ID_CTRL_TEST_W9 = 256,	 // 0 1 0 // test write to labview
	ID_GAIT_USE_MDP_DATA = 257,	 // 1 0 1 // true (1) if the robot should use gait parameters from the MDP   and     false(0) if it should use defaults from LabVIEW
	ID_LV_START,	 //    // 
	ID_LV_CH_0,	 //    // 
	ID_LV_CH_1,	 //    // 
	ID_LV_CH_2,	 //    // 
	ID_LV_CH_3,	 //    // 
	ID_LV_CH_4,	 //    // 
	ID_LV_CH_5,	 //    // 
	ID_LV_CH_6,	 //    // 
	ID_LV_CH_7,	 //    // 
	ID_LV_CH_8,	 //    // 
	ID_LV_CH_9,	 //    // 
	ID_LV_CH_10,	 //    // 
	ID_LV_CH_11,	 //    // 
	ID_LV_CH_12,	 //    // 
	ID_LV_CH_13,	 //    // 
	ID_LV_CH_14,	 //    // 
	ID_LV_CH_15,	 //    // 
	ID_LV_CH_16,	 //    // 
	ID_LV_CH_17,	 //    // 
	ID_LV_CH_18,	 //    // 
	ID_LV_CH_19,	 //    // 
	ID_LV_CH_20,	 //    // 
	ID_LV_CH_21,	 //    // 
	ID_LV_CH_22,	 //    // 
	ID_LV_CH_23,	 //    // 
	ID_LV_CH_24,	 //    // 
	ID_LV_CH_25,	 //    // 
	ID_LV_CH_26,	 //    // 
	ID_LV_CH_27,	 //    // 
	ID_LV_CH_28,	 //    // 
	ID_LV_CH_29,	 //    // 
	ID_LV_CH_30,	 //    // 
	ID_LV_CH_31,	 //    // 
	ID_LV_CH_32,	 //    // 
	ID_LV_CH_33,	 //    // 
	ID_LV_CH_34,	 //    // 
	ID_LV_CH_35,	 //    // 
	ID_LV_CH_36,	 //    // 
	ID_LV_CH_37,	 //    // 
	ID_LV_CH_38,	 //    // 
	ID_LV_CH_39,	 //    // 
	ID_LV_CH_40,	 //    // 
	ID_LV_CH_41,	 //    // 
	ID_LV_CH_42,	 //    // 
	ID_LV_CH_43,	 //    // 
	ID_LV_CH_44,	 //    // 
	ID_LV_CH_45,	 //    // 
	ID_LV_CH_46,	 //    // 
	ID_LV_CH_47,	 //    // 
	ID_LV_VERSION,	 //    // 
	ID_LAST
} CAN_ID;


#endif

