/*
  
  software_setup.c
  
  Board-dependent software setup.

*/

#include <includes.h>

/*********************  Variables  ****************************************/
char str[40];
int strlen;
// *******************************************
// Schedule
// *******************************************
VOID_VOID_F main_schedule[]={
  task_every_row, can_tx_angle, dn_sched_done, NULL,
  task_every_row, can_tx_rate, dn_sched_done, NULL,
  task_every_row, can_tx_left_heel_sense, dn_sched_done, NULL, // faster ( I am sending left only, right is gone worse)
  task_every_row, can_tx_motor_pos, dn_sched_done, NULL,
  task_every_row, can_tx_hs_right, dn_sched_done, NULL,
  task_every_row, can_tx_batt_power, dn_sched_done,NULL, // Reverted MPK's change 6/20/16 -- MWS. No longer using current at main brain.
  //task_every_row, can_tx_motor_current, dn_sched_done, NULL,  // Switched with batt_power on March 4, 2016 by MPK
  //task_every_row, can_tx_hs_left, dn_sched_done, NULL,  // commented out by anoop 20 feb 2013, because its not needed much
  //task_every_row, can_tx_left_heel_sense, dn_sched_done, NULL, // faster ( I am sending left only, right is gone worse)
  task_every_row, can_tx_right_heel_sense, dn_sched_done, NULL,  // faster
  task_every_row, run_occasionally, dn_sched_done, NULL,
  NULL
};

void task_every_row(void) 
{
  adcx_convert_all();
  adci_convert_all();
  qdc_tmr0_cap01_angle_update();
  qdc_tmr0_cap01_rate_update();
  qdc_tmr0_cap23_angle_update();
  qdc_tmr0_cap23_rate_update();
  can_rx_dispatch_all();
  hb_beat(); 
  mcu_led_update();
  ae_update();
  ls_update();
  dn_safety();
//  dn_reset_qec();
  error_update();
  can_tx2();

  adcx_conversion_wait();
  
  mc_compliant_control();

}

//A simple function to run other functions at a slow, not necessarily well-timed rate.
//Add additional else if statements below for each new function that needs to run.
//Be sure that the index i values are continuous from 0 to (number of functions - 1),
//because it will skip any functions after a gap.
void run_occasionally(void)
{
  static short i = 0;
  
  if (i == 0){error_send_next();}             // Send error from error buffer over the CAN bus
  else if (i == 1){can_tx_motor_vel();}       // Send motor angle over the CAN bus
  else if (i == 2){can_tx_motor_current();}        // Reverted MPK's March 4th change.
  //else if (i == 2){can_tx_batt_power();}  	   // Switched with motor current on March 4, 2016 by MPK
  else if (i == 3){ can_tx_hs_left();}  // // put by anoop 20 feb 2013, because its not needed to be fast enough (look into scheduler)
 // else if (i == 4){can_tx_batt_power();}      // Send motor controller input power over the CAN bus  // anoop commented this and transfered it into scheduler on 20 feb 2013
  else if (i == 4){can_tx_board_status();}    // Send board operation status code to main brain
  else if (i == 5){can_tx_batt_current();}    // Send battery current over the CAN bus
  else if (i == 6){can_tx_batt_voltage();}    // Send battery voltage over the CAN bus
  else if (i == 7){can_tx_hbridge_temp();}    // Send hbridge temp over the CAN bus
  else if (i == 8){can_tx_ls_right();}        // Send hbridge temp over the CAN bus
  else if (i == 9){can_tx_ls_left();}         // Send hbridge temp over the CAN bus
  else if (i == 10){can_tx_exec_time();}     
  else if (i == 11){can_tx_max_exec_time();}  
  else if (i == 12){can_tx_right_heel_sense();}
  else if (i == 13){dn_inner_feet_motor_encoder_rezero();}    
  else{i = -1;}
  ++i; 
}

/*********************  Software Initialization  **************************/
void init_software(void){
  // *********************************************
  // Standard board initialization. Do not modify. Unless you dare.
  // *********************************************
  asched_init(main_schedule,1);
  error_init(dn_error_transmit, asched_get_timestamp, BOARD_MCFI);
  hb_init(9, dn_blue_heartbeat, asched_get_timestamp); //heartbeat

  // *********************************************
  // Put user initialization code below here.  
  // *********************************************
  adci_init(ADCI_NULL, ADCI_NO_FILTER, ADCI_NULL, ADCI_NULL); //batt voltage on
  adcx_init(); 
  adcx_add_config(ADCX_CH_P4N5, ADCX_GAIN_2); //motor current
  adcx_add_config(ADCX_CH_P2N3, ADCX_GAIN_2); //battery current
  adcx_add_config(ADCX_CH_0, ADCX_GAIN_1); //left inner heel strike
  adcx_add_config(ADCX_CH_1, ADCX_GAIN_1); //right inner heel strike

  //Initialize incremental encoder position and rate calculations.
  //Put setup, smoothing/filtering, and calibration values here
  qdc_tmr0_init
  (
    60000000,     //Count frequency of timer 0 in cycles per second
    512,          //Pulses per revolution per channel (1X) of CAP01 encoder
    34,         //Gear ratio to obtain output shaft angle/rate, if desired, for CAP01 encoder.
    0.0005,       //Time per call to qdc_tmr0_cap01_rate_update in seconds
    4,            //Maximum exponential smoothing coefficient exponent for cap01. See instructions.
    4,            //Minimum exponential smoothing coefficient exponent for cap01
    2048,         //Pulses per revolution per channel (1X) of CAP23 encoder
    1.0,          //Gear reduction ratio to obtain output shaft angle/rate, if desired, for CAP23 encoder
    0.0005,       //Timer per call to qdc_tmr1_cap23_rate_update in seconds
    4,           //Maximum exponential smoothing coefficient exponent for cap23  See instructions.
    4             //Minimum exponential smoothing coefficient exponent for cap23
  );

  //Initialize motor controller
  mc_init
  (
    12.0, 
    8.0, 
    -2.0, 
    6.0, 
    -1.0,  
    3.1, 
    10.0,
    0.0,  // **** TEST CODE **** low Kp value 
   // 220.0, 
    20.0, 
    0.0, 
    300, 
    4, 
    20.0,
    MC_NORMAL, 
    &dn_get_raw_motor_current, 
    &dn_get_motor_pos_rads, 
    &dn_get_motor_vel_rads
  );

  ae_init_encoder(AE_2, 1572, 186, 8192);
  ls_init_switch(2, dn_get_p0); // id:0, left ankle
  ls_init_switch(2, dn_get_p1); // id:1, right angle
  ls_init_switch(10, dn_get_raw_right_hs); // id:2, right heel strike
  ls_init_switch(10, dn_get_raw_left_hs); //id:3, left heel strike
  
  mc_set_stiffness(0.0); //5.0
  mc_set_dampness(0.0); //0.2
  mc_set_command_current(0.0);
}


/*********************  Value Initialization  **************************/
void init_values(void){ //startup calibrations and the like
  ae_update();
  ae_wait();
  qdc_tmr0_cap01_set_angle_float(dn_get_ankle_pos_rads());
  qdc_tmr0_cap23_set_angle_float(dn_get_ankle_pos_rads());
}
