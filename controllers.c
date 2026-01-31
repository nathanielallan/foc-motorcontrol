/* Simple PI controllers for current and speed */
typedef struct {
    float kp;
    float ki;
    float integrator;
    float out_min, out_max;
} pi_t;

void pi_init(pi_t *pi, float kp, float ki, float out_min, float out_max)
{
    pi->kp = kp; pi->ki = ki; pi->integrator = 0;
    pi->out_min = out_min; pi->out_max = out_max;
}

float pi_update(pi_t *pi, float setpoint, float measurement, float dt)
{
    float err = setpoint - measurement;
    float p = pi->kp * err;
    pi->integrator += err * pi->ki * dt;
    // anti-windup
    if (pi->integrator > pi->out_max) pi->integrator = pi->out_max;
    if (pi->integrator < pi->out_min) pi->integrator = pi->out_min;
    float out = p + pi->integrator;
    if (out > pi->out_max) out = pi->out_max;
    if (out < pi->out_min) out = pi->out_min;
    return out;
}