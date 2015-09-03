#include <includes.h>
#include <can_id.h>

// NOTE: Errors disabled - error_send in sched

extern int global_running;
float global_zero_angle; 
// **********************  Variables  ************************** 

// *******************************************
// Scheduler Variables
// *******************************************
#define SCHEDULE_TICK_DIVIDER 1 //number of system jiffies (should be 1 mS) per time slot
typedef TASK_PTR TP;
TASK_PTR main_schedule[]={
  (TP)hb_beat,(TP)task_every_row,(TP)task_can_transmit1,(TP)error_send_next,(TP)NULL,
  (TP)hb_beat,(TP)task_every_row,(TP)task_can_transmit2,(TP)NULL,
  (TP)hb_beat,(TP)task_every_row,(TP)task_can_transmit3,(TP)error_send_next,(TP)NULL,
  (TP)hb_beat,(TP)task_every_row,(TP)task_can_transmit4,(TP)NULL,
  (TASK_PTR)NULL
};
TASK_PTR wait_schedule[]={
  (TP)task_test_wait,(TP)NULL,
  (TP)task_test_wait,(TP)NULL,
  (TASK_PTR)NULL
};
TASK_PTR exit_schedule[]={
  (TP)st_off,(TP)NULL,
  (TP)st_off,(TP)NULL,
  (TASK_PTR)NULL
};
// *******************************************
// CAN Variables
// *******************************************
// ***** Transmit
CAN_FRAME_DESC motor_pos_tx_fd;
CAN_FRAME_DESC motor_vel_tx_fd;
CAN_FRAME_DESC hip_angle_tx_fd;
//CAN_FRAME_DESC shaft_vel_tx_fd;
CAN_FRAME_DESC motor_current_tx_fd;
CAN_FRAME_DESC battery_voltage_tx_fd;
//CAN_FRAME_DESC qec_subscribe_fd;
CAN_FRAME_DESC error_tx_fd;
CAN_FRAME_DESC mch_ready_tx_fd;
// ***** Receive
CAN_FRAME_DESC motor_current_rx_fd;
CAN_FRAME_DESC timestamp_rx_fd;
CAN_FRAME_DESC mc_shutdown_rx_fd;
CAN_FRAME_DESC mc_sleep_rx_fd;
CAN_FRAME_DESC mb_ready_rx_fd;
CAN_FRAME_DESC start_rx_fd;



#define RX_FRAME_BUF_LEN 8
CAN_RING rx_ring;
CAN_FRAME rx_frame_buf[RX_FRAME_BUF_LEN];
#define TX_FRAME_BUF_LEN 8
CAN_RING tx_ring1;
CAN_FRAME tx_frame_buf1[TX_FRAME_BUF_LEN];
CAN_RING tx_ring2;
CAN_FRAME tx_frame_buf2[TX_FRAME_BUF_LEN];
#define RX_LIST_SIZE 10
CAN_FRAME_DESC * rxlist[RX_LIST_SIZE];
#define RTR_LIST_SIZE 10
CAN_FRAME_DESC * rtrlist[RTR_LIST_SIZE];
void fill_descriptors(void);


// **********************  Software Initialization  **************************
void init_software(void){

  // *********************************************
  // Standard board initialization. Do not modify. Unless you dare.
  // *********************************************
  sched_init(wait_schedule,SCHEDULE_TICK_DIVIDER,1);
  st_init(wait_schedule, main_schedule, exit_schedule);
  error_init(dn_error_transmit, BOARD_MC_HIP);
  hb_init(500, dn_blue_heartbeat); //heartbeat

  // *********************************************
  // Put user initialization code below here.  
  // *********************************************
  adci_init(ADCI_NULL, ADCI_NO_FILTER, ADCI_NULL, ADCI_NULL); //batt voltage on
  adcx_init(); 
  adcx_add_config( ADCX_CH_P4N5, ADCX_GAIN_1); //motor current
 // adcx_add_config( ADCX_CH_P2N3, ADCX_GAIN_1); //battery current
  mc_init(12.0, 2.0, -2.0, 220.0, 20.0, 0.0, 250, 4, &dn_get_raw_motor_current, &dn_get_motor_position, &dn_get_motor_velocity);
  qec_init(QEC_NULL, QEC_4X, sched_auxiliary_tick); //QEC for hip motor angle is J9
  ae_init(1,0); //abs encoder for hip angle is on J3
  ls_init(2,2);
  
  mc_set_target_current(0.0);

  
  // *******************************************
  // Set up CAN
  // *******************************************
  fill_descriptors();

  // ******** CAN1 ***********
//  can_ring_init(&rx_ring,rx_frame_buf,RX_FRAME_BUF_LEN);
  can_rx_set_descriptors(rxlist, rtrlist);
  can_rx_set_chan_cfg(CHAN_CAN1,(volatile unsigned long *)0xE0044000,NULL,CAN_DISPATCH_AUTO);

  can_ring_init(&tx_ring1,tx_frame_buf1,TX_FRAME_BUF_LEN);
  can_tx_set_chan_cfg(CHAN_CAN1,(volatile unsigned long *)0xE0044000,&tx_ring1);

   // ******** CAN2 ***********
//  can_ring_init(&rx_ring,rx_frame_buf,RX_FRAME_BUF_LEN);
  can_rx_set_chan_cfg(CHAN_CAN2,(volatile unsigned long *)0xE0048000,NULL,CAN_DISPATCH_AUTO);

  can_ring_init(&tx_ring2,tx_frame_buf2,TX_FRAME_BUF_LEN);
  can_tx_set_chan_cfg(CHAN_CAN2,(volatile unsigned long *)0xE0048000,&tx_ring2);
}

void init_values(void){ //startup calibrations and the like

  // ******** AE Zero Angle ***********
  ae_update();
  ae_wait();
  global_zero_angle = dn_get_shaft_pos_rads();
}

void fill_descriptors(void){

  // ************* First, fill regular RX descriptors ***************** //  
  int i = 0;
  
  rxlist[i++] = &motor_current_rx_fd;  //Motor Control - target current
  can_set_rx_descriptor_fi(&motor_current_rx_fd, ID_MCH_MOTOR_TARGET_CURRENT, mc_set_target_current, can_rx_setter_int_dummy);
  rxlist[i++] = &timestamp_rx_fd;  //Timestamp
  can_set_rx_descriptor_fi(&timestamp_rx_fd, ID_TIMESTAMP, can_rx_setter_float_dummy, sched_set_timestamp);
  rxlist[i++] = &mc_shutdown_rx_fd;  //Motor Shutdown
  can_set_rx_descriptor_ii(&mc_shutdown_rx_fd, ID_MCH_SHUTDOWN, mc_set_shutdown, can_rx_setter_int_dummy);
  rxlist[i++] = &mc_sleep_rx_fd;  //Motor Sleep
  can_set_rx_descriptor_ii(&mc_sleep_rx_fd, ID_MCH_SLEEP, mc_set_sleep, can_rx_setter_int_dummy);
  rxlist[i++] = &mb_ready_rx_fd;  //Main Brain Ready
  can_set_rx_descriptor_fi(&mb_ready_rx_fd, ID_MB_READY, dn_mb_ready, can_rx_setter_int_dummy);
  rxlist[i++] = &start_rx_fd;  //Main Brain Start
  can_set_rx_descriptor_fi(&start_rx_fd, ID_ALL_START, dn_start, can_rx_setter_int_dummy);
  rxlist[i++] = (CAN_FRAME_DESC *) 0;  //NULL Terminator

  if(i >= RX_LIST_SIZE) {
    error_occurred(ERROR_CAN_RX_LIST_OVERFLOW, PRIORITY_HIGH);
  }

  // ************* NOW, fill RTR descriptors ************************** //
  i = 0;
  //Encoder Info CAN frame - 2 floats
//  rtrlist[i++] = &qec_fd;
//  can_set_tx_descriptor_ff(&qec_fd, 0x101, CAN_CHAN_1,abs_pos_1, velocity_1);
  //NULL Terminator
  rtrlist[i++] = (CAN_FRAME_DESC *) 0;

  if(i >= RTR_LIST_SIZE) {
    error_occurred(ERROR_CAN_RTR_LIST_OVERFLOW, PRIORITY_HIGH);
  }

  // ************** NOW, fill standard non-RTR TX descriptors (not in a list) *** //
  can_set_tx_descriptor_fi(&motor_pos_tx_fd, ID_MCH_MOTOR_POSITION, CHAN_CAN1, dn_get_motor_pos_rads, sched_get_timestamp);
  can_set_tx_descriptor_fi(&motor_vel_tx_fd, ID_MCH_MOTOR_VELOCITY, CHAN_CAN1, dn_get_motor_vel_rads, sched_get_timestamp);
  can_set_tx_descriptor_fi(&hip_angle_tx_fd, ID_MCH_ANGLE, CHAN_CAN1, dn_get_shaft_pos_rads, sched_get_timestamp);
//  can_set_tx_descriptor_ff(&shaft_vel_tx_fd, 0x101, CHAN_CAN1, dn_get_shaft_vel_rads, can_tx_getter_float_dummy);
  can_set_tx_descriptor_fi(&motor_current_tx_fd, ID_MCH_MOTOR_CURRENT, CHAN_CAN1, dn_get_motor_current, sched_get_timestamp);
  can_set_tx_descriptor_fi(&battery_voltage_tx_fd, ID_BATTERY_VOLTAGE, CHAN_CAN1, dn_get_battery_voltage, sched_get_timestamp);
  can_set_tx_descriptor_fi(&mch_ready_tx_fd, ID_MCH_READY, CHAN_CAN1, can_tx_getter_float_dummy, can_tx_getter_int_dummy);
  can_set_tx_descriptor_iss(&error_tx_fd, ID_MCH_ERROR, CHAN_CAN1, error_get_time, error_get_id, error_get_freq); 
//  can_set_tx_descriptor_ff(&qec_subscribe_fd, 0x101, CHAN_CAN1,abs_pos_1, velocity_1);

}

/********************* TASKS ***********************/
void task_every_row(void) { 
  adcx_convert_all();
  mcu_led_update();
  ae_update();
  adcx_conversion_wait();
  dn_safety();
  mc_pid_current();
}

void task_can_transmit1(void){
  can_transmit(&motor_pos_tx_fd);
}

void task_can_transmit2(void){
  can_transmit(&motor_vel_tx_fd);
}

void task_can_transmit3(void){
  can_transmit(&hip_angle_tx_fd);
}

void task_can_transmit4(void){
  can_transmit(&motor_current_tx_fd);
}

void task_can_transmit_ready(void){
  can_transmit(&mch_ready_tx_fd);
}

void task_test_wait(void){
  static int wait_count = 0;
  wait_count++;
  if (wait_count == 1000*SCHED_SPEED){
    st_run();
  }
}

void task_test_exit(void){
  static int exit_count = 0;
  exit_count++;
  if (exit_count == 5000*SCHED_SPEED){
    st_exit();
  }
}

//TicToc
char tt_string[20];
int tt_str_length;
int tt_time = 0;
void tic(void){
  tt_time = T0TC;
}

void toc(void){
  tt_time = (T0TC - tt_time);
  if (tt_time <= 5000) {
    tt_str_length = sprintf(tt_string,"5000\n\r");
    uarti_tx_buf(tt_string, tt_str_length);
  } else if (tt_time <= 7500) {
    tt_str_length = sprintf(tt_string,"7500\n\r");
    uarti_tx_buf(tt_string, tt_str_length);
  } else if (tt_time <= 10000) {
    tt_str_length = sprintf(tt_string,"10000\n\r");
    uarti_tx_buf(tt_string, tt_str_length);
  } else if (tt_time <= 12500) {
    tt_str_length = sprintf(tt_string,"12500\n\r");
    uarti_tx_buf(tt_string, tt_str_length);
  } else if (tt_time <= 15000) {
    tt_str_length = sprintf(tt_string,"15000\n\r");
    uarti_tx_buf(tt_string, tt_str_length);
  } else {
    tt_str_length = sprintf(tt_string,"BIG\n\r");
    uarti_tx_buf(tt_string, tt_str_length);
  } 
  tt_time = 0;
}

void print_float(float f){
  printf("%f\n",f);
}

