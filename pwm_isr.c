/* This ISR is triggered at a safe point in PWM cycle after VADC sampling completes.
   It runs the inner current control and updates PWM duty cycles atomically.
*/
#include "adc_driver.h"
#include "transforms.h"
#include "controllers.h"
#include "svpwm.h"
#include "hall_driver.h"

void pwm_adc_sync_isr(void)   // bound to CCU4/VADC callback
{
    // 1) Read synchronous ADC results (phase currents IA, IB, maybe IC inferred)
    float ia = adc_get_phase_current_a();
    float ib = adc_get_phase_current_b();
    // float ic = -ia - ib;  // if only two shunts used

    // 2) Get electrical angle from Hall estimator (interpolated)
    float theta_e = hall_get_interpolated_angle();

    // 3) Clarke -> Park
    float i_alpha, i_beta, id, iq;
    clarke(ia, ib, &i_alpha, &i_beta);
    park(i_alpha, i_beta, theta_e, &id, &iq);

    // 4) Current PI controllers (d-q)
    float vd = pi_current_d_update(id_setpoint(), id);
    float vq = pi_current_q_update(iq_setpoint(), iq);

    // 5) Inverse Park -> alpha/beta voltages
    float v_alpha, v_beta;
    inv_park(vd, vq, theta_e, &v_alpha, &v_beta);

    // 6) SVPWM -> dutyA,B,C
    float duty_a, duty_b, duty_c;
    svpwm_calculate(v_alpha, v_beta, &duty_a, &duty_b, &duty_c);

    // 7) Update PWM (use hardware shadow registers for atomic update)
    pwm_update_duties(duty_a, duty_b, duty_c);
}