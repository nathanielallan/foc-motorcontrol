/* Hall decoding and angle estimate (coarse) + timestamping for speed. Use POSIF if available. */
#include <stdint.h>
#include "timing.h"

volatile uint32_t last_edge_time;
volatile float estimated_speed_rpm;
volatile float last_electrical_angle;

void hall_edge_isr(void)
{
    uint32_t now = tim_get_timestamp();
    uint32_t dt = now - last_edge_time;
    last_edge_time = now;

    // From the hall state, derive sector and convert to electrical angle (discrete 60deg increments)
    uint8_t hall_state = read_hall_pins();
    float sector_angle = hall_state_to_electrical_angle(hall_state);

    // Option: compute speed as 1/dt * (360 / sector_count) and apply filter
    estimated_speed_rpm = speed_filter_measurement(dt, sector_angle);

    // store angle and let PWM ISR interpolate between edges
    last_electrical_angle = sector_angle;
}

float hall_get_interpolated_angle(void)
{
    // Simple linear interpolation based on time since last edge and last edge interval
    float t_frac = tim_fraction_since(last_edge_time, last_edge_interval);
    return last_electrical_angle + sector_width * t_frac;
}