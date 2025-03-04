
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"

int main()
{
    // Initialize the device

    SYSCFG_DL_init();
    
    while(1)
    {
        // Run the device
        DL_GPIO_clearPins(LED1_PORT, LED1_PIN_22_PIN);
        delay_cycles(1000000);
        DL_GPIO_setPins(LED1_PORT, LED1_PIN_22_PIN);
        delay_cycles(1000000);
    }

}