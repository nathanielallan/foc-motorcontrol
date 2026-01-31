/* Minimal top-level: initialize, then wait; real-time control runs in interrupts */
#include "system_init.h"
#include "state_machine.h"
#include "comms.h"

int main(void)
{
    system_init();      // clocks, peripherals (VADC, CCU4, POSIF/GPIO), gpio leds
    comms_init();
    state_init();       // initialize state machine, set to INIT
    enable_global_interrupts();

    while (1) {
        // background tasks: telemetry, logging, slow supervisory tasks
        state_background_task();
        comms_process();
        // optionally enter low power or sleep between events
    }
    return 0;
}