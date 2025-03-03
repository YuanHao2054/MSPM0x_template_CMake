#include "dl_gpio.h"
#include "ti_msp_dl_config.h"

/* 自定义延时 */
void delay_ms(unsigned int ms)
{
    unsigned int i, j;

    for (i = 0; j < 8000; j++)
    {
        for (j = 0; j < 8000; j++)
        {
            __asm__("nop");
        }
    }
}


int main()
{
    // Initialize the device

    SYSCFG_DL_init();
    
    while(1)
    {
        // Run the device
        DL_GPIO_clearPins(LED1_PORT, LED1_PIN_22_PIN);
        delay_ms(1000);
        DL_GPIO_setPins(LED1_PORT, LED1_PIN_22_PIN);
        delay_ms(1000);
    }

}