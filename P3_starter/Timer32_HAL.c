/*
 * Timer32_HAL.c
 *
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "Timer32_HAL.h"

void startOneShotTimer0(unsigned int LoadVal)
{
    Timer32_setCount(TIMER32_0_BASE, LoadVal);
    Timer32_startTimer(TIMER32_0_BASE, true);
}

void startOneShotTimer1(unsigned int LoadVal)
{
    Timer32_setCount(TIMER32_1_BASE, LoadVal);
    Timer32_startTimer(TIMER32_1_BASE, true);
}

void InitTimer()
{

    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_0_BASE, 1);
    Timer32_startTimer(TIMER32_0_BASE, true);

    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_startTimer(TIMER32_1_BASE, true);
    Timer32_setCount(TIMER32_1_BASE, 1);
}

bool timer0Expired()
{
    return (Timer32_getValue(TIMER32_0_BASE) == 0);
}

void makeTimeToString(time_t time, int8_t *string)
{
    //For this mehtod we pass the time itself
    //this is exactly like make lap to string
    string[9] =  time.tenth + 48;//updated the 10th of a second
    string[7] =  (time.sec % 10) + 48;
    string[6] =  ((time.sec / 10) % 10 ) + 48;
    string[4] =  (time.min % 10) + 48;
    string[3] =  ((time.min / 10) % 10 ) + 48;
    string[1] =  (time.hour % 10) + 48;
    string[0] =  ((time.hour / 10) % 10 ) + 48;
}


void increaseTime(time_t *time_p)
    {

        time_p->tenth = time_p->tenth+1;
        if (time_p->tenth == 10)
        {
            time_p->tenth = 0;
            time_p->sec = time_p->sec + 1;
        }
        if(time_p->sec == 60)
        {
            time_p->sec = 0;
            time_p->min = time_p->min + 1;
        }
        if(time_p->min == 60)
        {
            time_p->min = 0;
            time_p->hour = time_p->hour + 1;
        }
        //how many seconds should pass for a min, how many mins should pass for an hour to be incremented
    //TODO: Finish this function
    }


