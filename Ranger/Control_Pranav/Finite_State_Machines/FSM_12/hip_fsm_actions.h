//static float abs_leg_angle_at_hs;
static float abs_leg_angle_at_ss0;
static float abs_hip_angle_at_ss0;
static float time_at_ss0;
static float ho_preswing_time;
static float hi_preswing_time;

const float e_avg = 1; //0 means estimates are not averaged.
                       //1 means the estimates are averaged
                       
const float e_avg_h = 0; //h is for hip ang and hip rate. Same meaning as above                  

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


// Heel-strike estimates start
//static float robot_absang_hs_in;
//static float robot_absang_hs_out;
//static float robot_absang_hs;
// Heel-strike estimates end

// Step estimates start
/*static float robot_velocity_step_in; //Not estimated
static float robot_velocity_step_out; //Not estimated
static float robot_velocity_step; //Not estimated

static float robot_stance_angle_step_in;
static float robot_stance_angle_step_out;
static float robot_stance_angle_step;

static float robot_hip_angle_step_in;
static float robot_hip_angle_step_out;
static float robot_hip_angle_step;*/

//static float robot_time_step_in;
//static float robot_time_step_out;
//static float robot_time_step;
// Step estimates end

// PS estimates start
static float robot_li_absang_phs;
static float robot_lo_absang_phs;

static float robot_li_angle_ps;
static float robot_lo_angle_ps;
static float robot_stance_angle_ps;

static float robot_li_velocity_ps;
static float robot_lo_velocity_ps;
static float robot_stance_velocity_ps; 
//PS estimates end

static float discrete_hi_aftermid_dcurrent;
static float discrete_ho_aftermid_dcurrent;

int ACT_HI_starthold_entry(void)
{ 
  return 1;
}

int ACT_HI_starthold(void)
{
  set_io_float(ID_MCH_STIFFNESS, get_io_float(ID_C_HI_SH_H_ANG));
  set_io_float(ID_MCH_DAMPNESS, get_io_float(ID_C_HI_SH_H_RATE));
  set_io_float(ID_MCH_COMMAND_CURRENT, get_io_float(ID_P_HI_SH_H_TANG) * get_io_float(ID_C_HI_SH_H_ANG));  
  
  
  return 1;
}

int ACT_HI_preswing_entry(void)
{
  hi_preswing_time = 0.0;

  //Estimation starts   
  //Remember: outer absolute angle here
  robot_lo_absang_phs = -get_io_float(ID_E_LO_ABSANG); //Is made positive
  
  /*robot_absang_hs_in  = get_io_float(ID_E_LI_ABSANG);
  robot_absang_hs = 0.5*((2-e_avg_h)*robot_absang_hs_in + e_avg_h*robot_absang_hs_out); 
  
  robot_hip_angle_step_in = -get_io_float(ID_E_LO_ABSANG); //Is made positive 
  robot_hip_angle_step = 0.5*((2-e_avg_h)*robot_hip_angle_step_in + e_avg_h*robot_hip_angle_step_out);
  set_io_float(ID_E_STEP_HIPANG,robot_hip_angle_step);
  
  robot_stance_angle_step_in = get_io_float(ID_E_LI_ABSANG); //Is positive (tilting fwd)
  robot_stance_angle_step = 0.5*((2-e_avg_h)*robot_stance_angle_step_in + e_avg_h*robot_stance_angle_step_out);
  set_io_float(ID_E_STEP_LEGANG,robot_stance_angle_step);
  
  robot_time_step_out = get_io_float(ID_E_STEP_TIME);
  float robot_time_step = 0.5*((2-e_avg_h)*robot_time_step_in + e_avg_h*robot_time_step_out);
  
  float time_from_mid_to_step = robot_time_step - get_io_float(ID_E_MIDSTANCE_TIME);
  float one_over_time_from_mid_to_step = 1000.0/time_from_mid_to_step;
  robot_velocity_step = get_io_float(ID_E_STEP_LEGANG)*one_over_time_from_mid_to_step;
  set_io_float(ID_E_STEP_LEGRATE,robot_velocity_step);*/

  //Estimation ends 
  
  
  set_io_float(ID_MCH_STIFFNESS, 0.0);
  set_io_float(ID_MCH_DAMPNESS, 0.0);
  set_io_float(ID_MCH_COMMAND_CURRENT, -get_io_float(ID_H_TEST1) ); //small negative current to prevent stubbing
  return 1;
}


int ACT_HI_preswing(void)
{
  hi_preswing_time = hi_preswing_time + 2.0;
  
  set_io_float(ID_MCH_STIFFNESS, 0.0);
  set_io_float(ID_MCH_DAMPNESS, 0.0);
  set_io_float(ID_MCH_COMMAND_CURRENT, 0.0 ); 
  return 1;
}

int ACT_HI_premid_entry(void)
{

  set_io_float(ID_E_H_PS_TIME,hi_preswing_time);
  
//Estimation starts
  abs_leg_angle_at_ss0 = -get_io_float(ID_E_LO_ABSANG);
 // hip_angle_at_hs = get_io_float(ID_MCH_ANGLE); //
 abs_hip_angle_at_ss0 = get_io_float(ID_MCH_ANGLE); //Is negative
 //abs_hip_angle_at_ss0 = -get_io_float(ID_E_LI_ABSANG); //Is made negative with the sign
 time_at_ss0 = get_io_float(ID_E_T_AFTER_HS);
 
 
  //Remember: estimate outer absolute angle and outer absolute rate here
  float one_over_ps_time = 1000.0/hi_preswing_time;
  robot_lo_angle_ps = -get_io_float(ID_E_LO_ABSANG); //Is made positive
  robot_lo_velocity_ps = (robot_lo_absang_phs - robot_lo_angle_ps)*one_over_ps_time; //Is positive
  
  robot_stance_angle_ps = 0.5*((2-e_avg_h)*robot_lo_angle_ps + e_avg_h*robot_li_angle_ps);
  robot_stance_velocity_ps =  0.5*((2-e_avg_h)*robot_lo_velocity_ps + e_avg_h*robot_li_velocity_ps);
  
  set_io_float(ID_E_SS0_LEGANG, robot_stance_angle_ps);
  set_io_float(ID_E_SS0_LEGRATE, robot_stance_velocity_ps);
 //Estimation ends
  

  return 1;
}

int ACT_HI_premid(void)
{

float command_current = 0.0;

  
   if  ( (get_io_float(ID_E_T_AFTER_HS)- time_at_ss0) < get_io_float(ID_P_H_PM_TIME) )
      command_current = get_io_float(ID_C_H_PM_H_RATE)*(get_io_float(ID_P_H_PM_H_TRATE) - get_io_float(ID_E_H_RATE)); //Velocity control based on hip rate
      

//  command_current = get_io_float(ID_A_H_PM_A0); 

  if (command_current<0)
      command_current = 0.0; 
  
    
   set_io_float(ID_MCH_STIFFNESS, 0.0 );
   set_io_float(ID_MCH_DAMPNESS, 0.0);      
   set_io_float(ID_MCH_COMMAND_CURRENT, command_current);
               

return 1;
}

int ACT_HI_premid_exit(void)
{
  return 1;
}


int ACT_HI_aftermid_entry(void)
{
  float time_from_ss0_to_mid_stance = get_io_float(ID_E_T_AFTER_HS)-time_at_ss0;
  float one_over_time_after_ss0 = 1000.00/time_from_ss0_to_mid_stance;
  
//Estimation starts  
  robot_velocity_mid_stance_in = abs_leg_angle_at_ss0*one_over_time_after_ss0;
  robot_velocity_mid_stance = 0.5*((2-e_avg)*robot_velocity_mid_stance_in + e_avg*robot_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_LEGRATE, robot_velocity_mid_stance);
     
  robot_hip_angle_mid_stance_in = get_io_float(ID_MCH_ANGLE); //Is positive if swing leg is in front
  //robot_hip_angle_mid_stance_in= -get_io_float(ID_E_LI_ABSANG); //Is positive if swing leg is leaning fwd
  robot_hip_angle_mid_stance = 0.5*((2-e_avg_h)*robot_hip_angle_mid_stance_in + e_avg_h*robot_hip_angle_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_HIPANG,robot_hip_angle_mid_stance); 

  robot_hip_velocity_mid_stance_in = (robot_hip_angle_mid_stance_in-abs_hip_angle_at_ss0)*one_over_time_after_ss0; //Is positive
  robot_hip_velocity_mid_stance = 0.5*((2-e_avg_h)*robot_hip_velocity_mid_stance_in+e_avg_h*robot_hip_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_HIPRATE,robot_hip_velocity_mid_stance);
  
  robot_time_mid_stance_in = time_from_ss0_to_mid_stance;
  robot_time_mid_stance = 0.5*((2-e_avg_h)*robot_time_mid_stance_in + e_avg_h*robot_time_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_TIME,robot_time_mid_stance);
//Estimation ends  
  

  discrete_hi_aftermid_dcurrent = 0.0;   
  if (((int) get_io_float(ID_D_H_ON)) == 1) //Discrete control
  { 
    float dcurrent1=0.0, dcurrent2 = 0.0, dcurrent3 = 0.0, dcurrent4 = 0.0;
   
    if (robot_velocity_mid_stance > get_io_float(ID_DP_F_PP_L_ABSRATE) ) //robot velocity is faster than target than change discrete control
      {   
        dcurrent3 = get_io_float(ID_DC_H_AM_L_ABSRATE)*(robot_velocity_mid_stance - get_io_float(ID_DP_F_PP_L_ABSRATE) );
        dcurrent4 = -get_io_float(ID_DC_H_AM_H_DRATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_DRATE)*robot_velocity_mid_stance);
      } 
     
   if (robot_velocity_mid_stance < get_io_float(ID_DP_F_PP_L_ABSRATE) ) //robot velocity is slower than target than 
      {   
        dcurrent2 =  -get_io_float(ID_DC_H_AM_H_RATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_RATE) ); 
      } 
  
  if (robot_hip_angle_mid_stance < get_io_float(ID_DP_H_AM_H_ANG) )     
     { 
            dcurrent1 = -get_io_float(ID_DC_H_AM_H_ANG)*(robot_hip_angle_mid_stance - get_io_float(ID_DP_H_AM_H_ANG) );
     }  
//      if (dcurrent1<0) dcurrent1 = 0.0; 
//      if (dcurrent2<0) dcurrent2 = 0.0;
//      if (dcurrent3<0) dcurrent3 = 0.0; 
//      if (dcurrent4<0) dcurrent4 = 0.0;
  
          
   discrete_hi_aftermid_dcurrent = dcurrent1 + dcurrent2 + dcurrent3 + dcurrent4;

   }
  
  set_io_float(ID_DA_H_AM_A0,discrete_hi_aftermid_dcurrent);
// Discrete control ends   
  
  
  return 1;
}

int ACT_HI_aftermid(void)
{

float command_current= 0.0;


 
   if  ( (get_io_float(ID_E_T_AFTER_HS)- time_at_ss0) < get_io_float(ID_P_H_PM_TIME) )
      command_current = get_io_float(ID_C_H_PM_H_RATE)*(get_io_float(ID_P_H_PM_H_TRATE) - get_io_float(ID_E_H_RATE)); //Velocity control based on hip rate
      
      
   if (command_current<0)
      command_current = 0.0; 
      
 
   set_io_float(ID_MCH_STIFFNESS, 0.0);
   set_io_float(ID_MCH_DAMPNESS, 0.0);      
   set_io_float(ID_MCH_COMMAND_CURRENT, command_current+discrete_hi_aftermid_dcurrent);
   

  
  return 1;
}

int ACT_HO_preswing_entry(void)
{

  ho_preswing_time = 0.0;
  
  
  //Estimation starts 
   //Remember: inner absolute angle here
   robot_li_absang_phs = -get_io_float(ID_E_LI_ABSANG); //Is made positive
  
  
  /*robot_absang_hs_out  = get_io_float(ID_E_LO_ABSANG);
  robot_absang_hs = 0.5*(e_avg_h*robot_absang_hs_in + (2-e_avg_h)*robot_absang_hs_out); 
  
  robot_hip_angle_step_out = -get_io_float(ID_E_LI_ABSANG); //Is made positive with the sign in front 
  robot_hip_angle_step = 0.5*(e_avg_h*robot_hip_angle_step_in + (2-e_avg_h)*robot_hip_angle_step_out);
  set_io_float(ID_E_STEP_HIPANG,robot_hip_angle_step);
  
  robot_stance_angle_step_out = get_io_float(ID_E_LO_ABSANG); //Is positive (tilting fwd)
  robot_stance_angle_step = 0.5*(e_avg_h*robot_stance_angle_step_in + (2-e_avg_h)*robot_stance_angle_step_out);
  set_io_float(ID_E_STEP_LEGANG,robot_stance_angle_step);
  
   robot_time_step_in = get_io_float(ID_E_STEP_TIME);
  float robot_time_step = 0.5*(e_avg_h*robot_time_step_in + (2-e_avg_h)*robot_time_step_out);
  
  float time_from_mid_to_step = robot_time_step - get_io_float(ID_E_MIDSTANCE_TIME);
  float one_over_time_from_mid_to_step = 1000.0/time_from_mid_to_step;
  robot_velocity_step = get_io_float(ID_E_STEP_LEGANG)*one_over_time_from_mid_to_step;
  set_io_float(ID_E_STEP_LEGRATE,robot_velocity_step);*/
  
  //Estimation ends 
  
 
  set_io_float(ID_MCH_STIFFNESS, 0.0);
  set_io_float(ID_MCH_DAMPNESS, 0.0);  
  set_io_float(ID_MCH_COMMAND_CURRENT, get_io_float(ID_H_TEST1) ); //small backward current to prevent stubbing
  return 1;
}


int ACT_HO_preswing(void)
{
  ho_preswing_time = ho_preswing_time + 2.0;

  set_io_float(ID_MCH_STIFFNESS, 0.0);
  set_io_float(ID_MCH_DAMPNESS, 0.0);  
  set_io_float(ID_MCH_COMMAND_CURRENT, 0.0  );
  return 1;
}

int ACT_HO_premid_entry(void)
{

set_io_float(ID_E_H_PS_TIME,ho_preswing_time);

//Estimation starts 
  abs_leg_angle_at_ss0 = -get_io_float(ID_E_LI_ABSANG);
  //hip_angle_at_hs = get_io_float(ID_MCH_ANGLE); //Is positive 
  abs_hip_angle_at_ss0 = -get_io_float(ID_MCH_ANGLE); //Is made negative 
  //abs_hip_angle_at_ss0 = -get_io_float(ID_E_LO_ABSANG); //Is made positive with the negative sign
  time_at_ss0 = get_io_float(ID_E_T_AFTER_HS);
  
   //Remember: inner absolute angle and inner absolute rate here
  float one_over_ps_time = 1000.0/ho_preswing_time;
  robot_li_angle_ps = -get_io_float(ID_E_LI_ABSANG); //Is made positive
  robot_li_velocity_ps = (robot_li_absang_phs - robot_li_angle_ps)*one_over_ps_time; //Is positive
  
  robot_stance_angle_ps = 0.5*(e_avg_h*robot_lo_angle_ps + (2-e_avg_h)*robot_li_angle_ps);
  robot_stance_velocity_ps =  0.5*(e_avg_h*robot_lo_velocity_ps + (2-e_avg_h)*robot_li_velocity_ps);
  
  set_io_float(ID_E_SS0_LEGANG, robot_stance_angle_ps);
  set_io_float(ID_E_SS0_LEGRATE, robot_stance_velocity_ps);  
//Estimation ends 
  
  return 1;
}

int ACT_HO_premid(void)
{

float command_current=0.0;
float stiffness = 0.0;   
float const_current = 0.0;
float dampness = 0.0;
  
  if  ( (get_io_float(ID_E_T_AFTER_HS)- time_at_ss0) < get_io_float(ID_P_H_PM_TIME) )
    command_current = get_io_float(ID_C_H_PM_H_RATE)*(-get_io_float(ID_P_H_PM_H_TRATE) - get_io_float(ID_E_H_RATE)); 
    
    if (command_current>0)
      command_current = 0.0; 
    

    // command_current = -get_io_float(ID_A_H_PM_A0); 
  
   /*if (get_io_float(ID_MCH_ANGLE)>=0.0) //add stiffness in first half
       stiffness = get_io_float(ID_C_H_OUTER_STIFFNESS);
   else //or add constant current in the second half
        {
         const_current = get_io_float(ID_A_H_PM_A0);
         dampness = -get_io_float(ID_C_H_OUTER_DAMPNESS);
         }*/
              
   stiffness = get_io_float(ID_C_H_OUTER_STIFFNESS);                
   set_io_float(ID_MCH_STIFFNESS, stiffness); 
   set_io_float(ID_MCH_DAMPNESS, dampness);   
   set_io_float(ID_MCH_COMMAND_CURRENT, command_current-const_current);   

  return 1;
}


int ACT_HO_premid_exit(void)
{
  return 1;
}


int ACT_HO_aftermid_entry(void)
{
  float time_from_ss0_to_mid_stance = get_io_float(ID_E_T_AFTER_HS)-time_at_ss0;
  float one_over_time_after_ss0 = 1000.00/time_from_ss0_to_mid_stance;
   
//Estimation starts 
  robot_velocity_mid_stance_out = abs_leg_angle_at_ss0*one_over_time_after_ss0;
  robot_velocity_mid_stance = 0.5*(e_avg*robot_velocity_mid_stance_in + (2-e_avg)*robot_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_LEGRATE,robot_velocity_mid_stance);
  
  robot_hip_angle_mid_stance_out = -get_io_float(ID_MCH_ANGLE); //Is negative if swing leg is in front
 // robot_hip_angle_mid_stance_out  = -get_io_float(ID_E_LO_ABSANG); //Is positive if the hip is leaning in front
  robot_hip_angle_mid_stance = 0.5*(e_avg_h*robot_hip_angle_mid_stance_in + (2-e_avg_h)*robot_hip_angle_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_HIPANG,robot_hip_angle_mid_stance); 

  robot_hip_velocity_mid_stance_out = (robot_hip_angle_mid_stance_out-abs_hip_angle_at_ss0)*one_over_time_after_ss0; //Is positive
  robot_hip_velocity_mid_stance = 0.5*(e_avg_h*robot_hip_velocity_mid_stance_in+(2-e_avg_h)*robot_hip_velocity_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_HIPRATE,robot_hip_velocity_mid_stance);
  
  robot_time_mid_stance_out = time_from_ss0_to_mid_stance;
  robot_time_mid_stance = 0.5*(e_avg_h*robot_time_mid_stance_in + (2-e_avg_h)*robot_time_mid_stance_out);
  set_io_float(ID_E_MIDSTANCE_TIME,robot_time_mid_stance);
//Estimation ends
  
 
  discrete_ho_aftermid_dcurrent = 0.0;   
  if (((int) get_io_float(ID_D_H_ON)) == 1) //Discrete control
  { 
  
    float dcurrent1=0.0, dcurrent2 = 0.0, dcurrent3 = 0.0, dcurrent4 = 0.0;
  
        
    if (robot_velocity_mid_stance > get_io_float(ID_DP_F_PP_L_ABSRATE) ) //robot velocity is faster than target than change discrete control
      {   
        dcurrent3 = get_io_float(ID_DC_H_AM_L_ABSRATE)*(robot_velocity_mid_stance - get_io_float(ID_DP_F_PP_L_ABSRATE) );
        dcurrent4 = -get_io_float(ID_DC_H_AM_H_DRATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_DRATE)*robot_velocity_mid_stance);
      } 
      
   if (robot_velocity_mid_stance < get_io_float(ID_DP_F_PP_L_ABSRATE) ) //robot velocity is slower than target than 
      {   
        dcurrent2 =  -get_io_float(ID_DC_H_AM_H_RATE)*(robot_hip_velocity_mid_stance - get_io_float(ID_DP_H_AM_H_RATE) ); 
      } 
     
     if (robot_hip_angle_mid_stance < get_io_float(ID_DP_H_AM_H_ANG) )      
      { 
            dcurrent1 = -get_io_float(ID_DC_H_AM_H_ANG)*(robot_hip_angle_mid_stance - get_io_float(ID_DP_H_AM_H_ANG) );
      }
  
   
//      if (dcurrent1<0) dcurrent1 = 0.0; 
//      if (dcurrent2<0) dcurrent2 = 0.0;
//      if (dcurrent3<0) dcurrent3 = 0.0; 
//      if (dcurrent4<0) dcurrent4 = 0.0; 


   discrete_ho_aftermid_dcurrent = dcurrent1 + dcurrent2 + dcurrent3 + dcurrent4;
      
   }

  set_io_float(ID_DA_H_AM_A0,discrete_ho_aftermid_dcurrent);
    
  return 1;
}

int ACT_HO_aftermid(void)
{

float command_current= 0.0;
float stiffness = 0.0;
float const_current = 0.0;
float dampness = 0.0; 


 if  ( (get_io_float(ID_E_T_AFTER_HS)- time_at_ss0) < get_io_float(ID_P_H_PM_TIME) )
    command_current = get_io_float(ID_C_H_PM_H_RATE)*(-get_io_float(ID_P_H_PM_H_TRATE) - get_io_float(ID_E_H_RATE));  
    
   if (command_current>0)
      command_current = 0.0; 

   /*if (get_io_float(ID_MCH_ANGLE)>=0.0) //add stiffness in first half
       stiffness = get_io_float(ID_C_H_OUTER_STIFFNESS);
    else //or add constant current in the second half
    {
        const_current = get_io_float(ID_A_H_PM_A0);
        dampness = -get_io_float(ID_C_H_OUTER_DAMPNESS);
    }*/
        
  stiffness = get_io_float(ID_C_H_OUTER_STIFFNESS);             
  set_io_float(ID_MCH_STIFFNESS, stiffness);  
  set_io_float(ID_MCH_DAMPNESS, dampness);
  set_io_float(ID_MCH_COMMAND_CURRENT, command_current-discrete_ho_aftermid_dcurrent-const_current);   

 
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

int ACT_H_stop(void)
{
  set_io_float(ID_MCH_STIFFNESS, 0);
  set_io_float(ID_MCH_DAMPNESS, 0);
  set_io_float(ID_MCH_COMMAND_CURRENT, 0.0);
  return 1;
}

