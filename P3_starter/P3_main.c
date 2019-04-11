#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "ButtonLED_HAL.h"
#include "graphics_HAL.h"
#include "Timer32_HAL.h"
#include "ADC_HAL.h"
//#include "setup.c"

#define COUNT 144000000



void initialize();
void game();

int main(void)
{
    Graphics_Context g_sContext;

    initialize();
    InitGraphics(&g_sContext);

    extern Graphics_Image BowlingEvolution8BPP_UNCOMP;

    //Graphics_drawImage(&g_sContext, &BowlingEvolution8BPP_UNCOMP, 0, 0);

    //startOneShotTimer0(COUNT);

    /*while(!timer0Expired())
    {

    }

    if(timer0Expired())
    {*/
        while (1)
        {
            game();
        }
    //}
}

void initialize()
{
    // stop the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // initialize the boosterPack LEDs and turn them off except for red LED
    initialize_BoosterpackLED_red();
    initialize_BoosterpackLED_green();
    initialize_BoosterpackLED_blue();
    turnOff_BoosterpackLED_red();
    turnOff_BoosterpackLED_green();
    turnOff_BoosterpackLED_blue();

    // initialize the Launchpad buttons
    initialize_LaunchpadLeftButton();
    initialize_LaunchpadRightButton();


    // Initialize the timers needed for debouncing
    Timer32_initModule(TIMER32_0_BASE, // There are two timers, we are using the one with the index 0
                       TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
                       TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
                       TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer

    Timer32_initModule(TIMER32_1_BASE, // There are two timers, we are using the one with the index 1
                       TIMER32_PRESCALER_1, // The prescaler value is 1; The clock is not divided before feeding the counter
                       TIMER32_32BIT, // The counter is used in 32-bit mode; the alternative is 16-bit mode
                       TIMER32_PERIODIC_MODE); //This options is irrelevant for a one-shot timer


    initADC();
    initJoyStick();
    startADC();
}


