#pragma once

/*----------------------------------------------------------------------
------------------------------------------------------------------------
  Define the struct for the PI controller used in the speed and current
  control loops.
------------------------------------------------------------------------
----------------------------------------------------------------------*/
typedef struct {
    float kp;
    float ki;
    float integrator;
    float out_min;
    float out_max;
} pi_t;

void  pi_init(pi_t* pi, float kp, float ki, float out_min, float out_max);
float pi_update(pi_t* pi, float setpoint, float measurement, float dt);
void  pi_reset(pi_t* pi);
