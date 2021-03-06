float hip_speed_factor;
float fi_speed_factor;
float fo_speed_factor;
int stop_walk_on_inner_flag;
int stop_walk_on_outer_flag;
int stop_prepush_flag;
int start_walk_flag;

static float abs_leg_angle_at_hs;
static float hip_angle_at_hs;

// These estimates can also be done in estimator.c
// Mid-stance estimates start
static float robot_velocity_mid_stance_in;
static float robot_velocity_mid_stance_out;
static float robot_velocity_mid_stance;

static float robot_hip_velocity_mid_stance_in;
static float robot_hip_velocity_mid_stance_out;
static float robot_hip_velocity_mid_stance;

static float robot_hip_angle_mid_stance_in;
static float robot_hip_angle_mid_stance_out;
static float robot_hip_angle_mid_stance;

static float robot_time_mid_stance_in;
static float robot_time_mid_stance_out;
static float robot_time_mid_stance;
// Mid-stance estimates end

// These estimates can also be done in estimator.c
// Step estimates start
static float robot_velocity_step_in;
static float robot_velocity_step_out;
static float robot_velocity_step;

static float robot_stance_angle_step_in;
static float robot_stance_angle_step_out;
static float robot_stance_angle_step;

static float robot_hip_angle_step_in;
static float robot_hip_angle_step_out;
static float robot_hip_angle_step;

static float robot_time_step_in;
static float robot_time_step_out;
//static float robot_time_step;
// Step estimates end


static float discrete_hi_aftermid_dcurrent;
static float discrete_ho_aftermid_dcurrent;

int ACT_HI_starthold_entry(void)
{ 
  return 1;
}

int ACT_HI_starthold(void)
{
  stop_walk_on_inner_flag = 0; //don't stop walking.
  stop_walk_on_outer_flag = 0;
  stop_prepush_flag = 0;
  
  set_io_float(ID_MCH_STIFFNESS, get_io_float(ID_C_HI_SH_H_ANG));
  set_io_float(ID_MCH_DAMPNESS, get_io_float(ID_C_HI_SH_H_RATE));
  set_io_float(ID_MCH_COMMAND_CURRENT, get_io_float(ID_P_HI_SH_H_TANG) * get_io_float(ID_C_HI_SH_H_ANG));  
  return 1;
}

int ACT_HI_preswing(void)
{
  set_io_float(ID_MCH_COMMAND_CURRENT, 0.0);
  return 1;
}

int ACT_HI_premid_entry(void)
{

  
//Estimation starts
  abs_leg_angle_at_hs = -get_io_float(ID_E_LO_ABSANG);
  hip_angle_at_hs = get_io_float(ID_MCH_ANGLE); //Is negative
  
  robot_hip_angle_step_out = -get_io_float(ID_MCH_ANGLE); //Is negative
  robot_hip_angle_step = 0.5*(robot_hip_angle_step_in + robot_hip_angle_step_out);
  set_io_float(ID_E_STEP_HIPANG,robot_hip_angle_step);
  
  robot_stance_angle_step_in = get_io_float(ID_E_LI_ABSANG); //Is positive (tilting fwd)
  robot_stance_angle_step = 0.5*(robot_stance_angle_step_in + robot_stance_angle_step_out);
  set_io_float(ID_E_STEP_LEGANG,robot_stance_angle_step);
  
  robot_time_step_out = get_io_float(ID_E_STEP_TIME);
  float robot_time_step = 0.5*(robot_time_step_in + robot_time_step_out);
  
  float time_from_mid_to_step = robot_time_step+ - get_io_float(ID_E_MIDSTANCE_TIME);
  robot_velocity_step = 1000.00*get_io_float(ID_E_STEP_LEGANG)/time_from_mid_to_step;
  set_io_float(ID_E_STEP_LEGRATE,robot_velocity_step);
//Estimation ends
  

  return 1;
}

int ACT_HI_premid(void)
{


  
float command_current;


   if (get_io_float(ID_E_T_AFTER_HS) < get_io_float(ID_P_H_PM_TIME)) //Apply for time ID_P_H_PM_TIME
   { 
      command_current = hip_speed_factor*get_io_float(ID_C_H_PM_H_RATE)*(get_io_float(ID_P_H_PM_H_TRATE)*hip_speed_factor - get_io_float(ID_E_H_RATE)); //Velocity control based on hip rate
   } 
   else 
   {
      command_current = 0.0;
   } 

         
   set_io_float(ID_MCH_STIFFNESS, 0);
   set_io_float(ID_MCH_DAMPNESS, 0);      
   set_io_float(ID_MCH_COMMAND_CURRENT, command_current);
               

return 1;
}

int ACT_HI_premid_exit(void)
{
hip_speed_factor = get_io_float(ID_T_TEST1); 
fo_speed_factor = get_io_float(ID_T_TEST2);
fi_speed_factor = get_io_float(ID_T_TEST2);
stop_walk_on_outer_flag = (int) get_io_float(ID_T_TEST3);
stop_prepush_flag = (int) get_io_float(ID_T_TEST4);

return 1;
} 


int ACT_HI_aftermid_entry(void)
{
  
//Estimation starts  
  robot_velocity_mid_stance_in = 1000.00*abs_leg_angle_at_hs/get_io_float(ID_E_T_AFTER_HS);
  robot_velocity_mid_stance = 0.5*(robot_velocity_mid_stance_in + robot_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_LEGRATE, robot_velocity_mid_stance);
  
  robot_hip_angle_mid_stance_in = get_io_float(ID_MCH_ANGLE); //Is positive if swing leg is in front
  robot_hip_angle_mid_stance = 0.5*(robot_hip_angle_mid_stance_in + robot_hip_angle_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_HIPANG,robot_hip_angle_mid_stance); 

  robot_hip_velocity_mid_stance_in = 1000.00*(get_io_float(ID_MCH_ANGLE)- hip_angle_at_hs)/get_io_float(ID_E_T_AFTER_HS); //Is positive
  robot_hip_velocity_mid_stance = 0.5*(robot_hip_velocity_mid_stance_in+robot_hip_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_HIPRATE,robot_hip_velocity_mid_stance);
  
  robot_time_mid_stance_in = get_io_float(ID_E_T_AFTER_HS);
  robot_time_mid_stance = 0.5*(robot_time_mid_stance_in + robot_time_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_TIME,robot_time_mid_stance);
//Estimation ends  
  

// Discrete control 
  if (((int) get_io_float(ID_D_H_ON)) == 1)
  {
     //Put discrete actions here
    float discrete_hi_aftermid_dcurrent1 = -get_io_float(ID_DC_H_AM_H_RATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_RATE) );
    float discrete_hi_aftermid_dcurrent2 = -get_io_float(ID_DC_H_AM_H_DRATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_DRATE)*robot_velocity_mid_stance);
    if (discrete_hi_aftermid_dcurrent1< 0.0) {discrete_hi_aftermid_dcurrent1 = 0.0;}
    if (discrete_hi_aftermid_dcurrent2< 0.0) {discrete_hi_aftermid_dcurrent2 = 0.0;}
    discrete_hi_aftermid_dcurrent = discrete_hi_aftermid_dcurrent1 + discrete_hi_aftermid_dcurrent2;
    //discrete_hi_aftermid_dcurrent = 0.0;
    
  }
  else
  {
    discrete_hi_aftermid_dcurrent = 0.0;
  }
  
  set_io_float(ID_DA_H_AM_A0,discrete_hi_aftermid_dcurrent);
// Discrete control ends   
  
  
  return 1;
}

int ACT_HI_aftermid(void)
{

float command_current=0.0;

  
   if (get_io_float(ID_E_T_AFTER_HS) < 600.00)    //Apply discrete control only for 600 ms from previous impact
   {  
       if (get_io_float(ID_E_T_AFTER_HS) < get_io_float(ID_P_H_PM_TIME) ) //velocity control only for time ID_P_H_PM_TIME
       {                       
          command_current = hip_speed_factor*get_io_float(ID_C_H_PM_H_RATE)*(get_io_float(ID_P_H_PM_H_TRATE)*hip_speed_factor - get_io_float(ID_E_H_RATE))
                            +discrete_hi_aftermid_dcurrent;              
       } 
       else 
       {
          command_current = 0.0+discrete_hi_aftermid_dcurrent;
       }
   }  
   else 
        { command_current = 0.0; }
   

     
   set_io_float(ID_MCH_STIFFNESS, 0);
   set_io_float(ID_MCH_DAMPNESS, 0);      
   set_io_float(ID_MCH_COMMAND_CURRENT, command_current);
  
  return 1;
}



int ACT_HO_preswing(void)
{
  set_io_float(ID_MCH_STIFFNESS, 0);
  set_io_float(ID_MCH_DAMPNESS, 0);  
  set_io_float(ID_MCH_COMMAND_CURRENT, 0.0);
  return 1;
}

int ACT_HO_premid_entry(void)
{ 

//Estimation starts 
  abs_leg_angle_at_hs = -get_io_float(ID_E_LI_ABSANG);
  hip_angle_at_hs = get_io_float(ID_MCH_ANGLE); //Is positive 
  
  robot_hip_angle_step_in = get_io_float(ID_MCH_ANGLE); //Is positive
  robot_hip_angle_step = 0.5*(robot_hip_angle_step_in + robot_hip_angle_step_out);
  set_io_float(ID_E_STEP_HIPANG,robot_hip_angle_step);
  
  robot_stance_angle_step_out = get_io_float(ID_E_LO_ABSANG); //Is positive (tilting fwd)
  robot_stance_angle_step = 0.5*(robot_stance_angle_step_in + robot_stance_angle_step_out);
  set_io_float(ID_E_STEP_LEGANG,robot_stance_angle_step);
  
  robot_time_step_in = get_io_float(ID_E_STEP_TIME);
  float robot_time_step = 0.5*(robot_time_step_in + robot_time_step_out);
  
  float time_from_mid_to_step = robot_time_step - get_io_float(ID_E_MIDSTANCE_TIME);
  robot_velocity_step = 1000.00*get_io_float(ID_E_STEP_LEGANG)/time_from_mid_to_step;
  set_io_float(ID_E_STEP_LEGRATE,robot_velocity_step);
//Estimation ends 
  
  return 1;
}

int ACT_HO_premid(void)
{

float command_current;

   if (get_io_float(ID_E_T_AFTER_HS) < get_io_float(ID_P_H_PM_TIME)) //Apply velocity control only for time ID_P_H_PM_TIME
     {
      command_current = hip_speed_factor*get_io_float(ID_C_H_PM_H_RATE)*(-get_io_float(ID_P_H_PM_H_TRATE)*hip_speed_factor - get_io_float(ID_E_H_RATE));
     }
   else
      {command_current =0.0;}    
      
              
   set_io_float(ID_MCH_STIFFNESS, get_io_float(ID_C_H_OUTER_STIFFNESS)); //This stiffness makes the outer leg swing with the same time period as that of inner leg
   set_io_float(ID_MCH_DAMPNESS, 0);   
   set_io_float(ID_MCH_COMMAND_CURRENT, command_current);   

  return 1;
}


int ACT_HO_premid_exit(void)
{
hip_speed_factor = get_io_float(ID_T_TEST1); 
fo_speed_factor = get_io_float(ID_T_TEST2);
fi_speed_factor = get_io_float(ID_T_TEST2);
stop_walk_on_inner_flag = (int) get_io_float(ID_T_TEST3);
stop_prepush_flag = (int) get_io_float(ID_T_TEST4);


return 1;
} 

int ACT_HO_aftermid_entry(void)
{
  
//Estimation starts 
  robot_velocity_mid_stance_out = 1000.0*abs_leg_angle_at_hs/get_io_float(ID_E_T_AFTER_HS);
  robot_velocity_mid_stance = 0.5*(robot_velocity_mid_stance_in + robot_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_LEGRATE,robot_velocity_mid_stance);
  
  robot_hip_angle_mid_stance_out = -get_io_float(ID_MCH_ANGLE); //Is negative if swing leg is in front
  robot_hip_angle_mid_stance = 0.5*(robot_hip_angle_mid_stance_out+robot_hip_angle_mid_stance_in);
  set_io_float(ID_E_MIDSTANCE_HIPANG,robot_hip_angle_mid_stance); 

  robot_hip_velocity_mid_stance_out = -1000.00*(get_io_float(ID_MCH_ANGLE)-hip_angle_at_hs)/get_io_float(ID_E_T_AFTER_HS); //Is negative so sign
  robot_hip_velocity_mid_stance = 0.5*(robot_hip_velocity_mid_stance_in+robot_hip_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_HIPRATE,robot_hip_velocity_mid_stance);
  
  robot_time_mid_stance_out = get_io_float(ID_E_T_AFTER_HS);
  robot_time_mid_stance = 0.5*(robot_time_mid_stance_in + robot_time_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_TIME,robot_time_mid_stance);
//Estimation ends
  
  
  if (((int) get_io_float(ID_D_H_ON)) == 1) //Discrete control
  {
    float discrete_ho_aftermid_dcurrent1 = -get_io_float(ID_DC_H_AM_H_RATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_RATE) );
    float discrete_ho_aftermid_dcurrent2 = -get_io_float(ID_DC_H_AM_H_DRATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_DRATE)*robot_velocity_mid_stance);
    if (discrete_ho_aftermid_dcurrent1< 0.0) {discrete_ho_aftermid_dcurrent1 = 0.0;}
    if (discrete_ho_aftermid_dcurrent2< 0.0) {discrete_ho_aftermid_dcurrent2 = 0.0;}
    discrete_ho_aftermid_dcurrent = discrete_ho_aftermid_dcurrent1 + discrete_ho_aftermid_dcurrent2;
    
  }
  else
  {
    discrete_ho_aftermid_dcurrent = 0.0;
  }
  
  set_io_float(ID_MCH_STIFFNESS, get_io_float(ID_C_H_OUTER_STIFFNESS));
  set_io_float(ID_MCH_DAMPNESS, 0);
  set_io_float(ID_DA_H_AM_A0,discrete_ho_aftermid_dcurrent);
    
  return 1;
}

int ACT_HO_aftermid(void)
{

float command_current=0.0;

  
if (get_io_float(ID_E_T_AFTER_HS) < 600.00) //Apply discrete control only for 600 ms
  {
     if (get_io_float(ID_E_T_AFTER_HS) < get_io_float(ID_P_H_PM_TIME))
       {                   
        command_current = hip_speed_factor*get_io_float(ID_C_H_PM_H_RATE)*(-get_io_float(ID_P_H_PM_H_TRATE)*hip_speed_factor - get_io_float(ID_E_H_RATE)) 
                          -discrete_ho_aftermid_dcurrent; 
       }
       else
       {
       command_current = -discrete_ho_aftermid_dcurrent;
       }
  }
else 
     {command_current = 0.0;}
 

  set_io_float(ID_MCH_STIFFNESS, get_io_float(ID_C_H_OUTER_STIFFNESS));
  set_io_float(ID_MCH_DAMPNESS, 0);
  set_io_float(ID_MCH_COMMAND_CURRENT, command_current);   

  return 1;
}

int ACT_HI_ehold_entry(void)
{

  return 1;
}

int ACT_HI_ehold(void)
{ 
   set_io_float(ID_MCH_STIFFNESS, get_io_float(ID_C_H_EH_H_ANG));
   set_io_float(ID_MCH_DAMPNESS, get_io_float(ID_C_H_EH_H_RATE));
   set_io_float(ID_MCH_COMMAND_CURRENT, get_io_float(ID_P_H_EH_H_TANG) * get_io_float(ID_C_H_EH_H_ANG)); 
  
  return 1;
}

int ACT_HO_ehold_entry(void)
{
  return 1;
}

int ACT_HO_ehold(void)
{
    set_io_float(ID_MCH_STIFFNESS, get_io_float(ID_C_H_EH_H_ANG));
    set_io_float(ID_MCH_DAMPNESS, get_io_float(ID_C_H_EH_H_RATE));
    set_io_float(ID_MCH_COMMAND_CURRENT, - get_io_float(ID_P_H_EH_H_TANG) * get_io_float(ID_C_H_EH_H_ANG));  

  return 1;
}

///////////////////////////////
int ACT_HI_stophold_entry(void)
{

  return 1;
}

int ACT_HI_stophold(void)
{ 
   set_io_float(ID_MCH_STIFFNESS, 0.0);
   set_io_float(ID_MCH_DAMPNESS, 0.0);
   
   if ( get_io_float(ID_E_LO_ABSANG) < 0.0 ) //premid mid
      set_io_float(ID_MCH_COMMAND_CURRENT, 0.0 ); 
   else //aftermid
      set_io_float(ID_MCH_COMMAND_CURRENT, get_io_float(ID_H_TEST2) );
      
   //Check for start walking flag
   start_walk_flag = (int) get_io_float(ID_T_TEST5); //flag to indicate start. 0 means don't start.    
  
  return 1;
}

int ACT_HO_stophold_entry(void)
{

  return 1;
}

int ACT_HO_stophold(void)
{ 
   set_io_float(ID_MCH_STIFFNESS, 0.0);
   set_io_float(ID_MCH_DAMPNESS, 0.0);
   
   if ( get_io_float(ID_E_LI_ABSANG) < 0.0 ) //premid mid
      set_io_float(ID_MCH_COMMAND_CURRENT, 0.0 ); 
   else //aftermid
      set_io_float(ID_MCH_COMMAND_CURRENT, -get_io_float(ID_H_TEST2) );
      
   //Check for start walking flag
   start_walk_flag = (int) get_io_float(ID_T_TEST5); //flag to indicate start. 0 means don't start.   
  
  
  return 1;
}


/////////////////////////

int ACT_H_stop(void)
{
  set_io_float(ID_MCH_STIFFNESS, 0);
  set_io_float(ID_MCH_DAMPNESS, 0);
  set_io_float(ID_MCH_COMMAND_CURRENT, 0.0);
  return 1;
}

