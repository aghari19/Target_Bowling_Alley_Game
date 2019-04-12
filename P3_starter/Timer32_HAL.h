/*
 * Timer32_HAL.h
 *
 */

#ifndef TIMER32_HAL_H_
#define TIMER32_HAL_H_
typedef struct
{
    uint32_t timerID;
    uint32_t sourceClockF_inMHz;
    uint32_t prescalar;
    uint32_t loadValue;
    uint32_t rolloverCount;
    uint32_t interruptNumber;
} HWTimer_t;

typedef struct
{
    HWTimer_t   *hwtimer_p;             // hardware timer used as basis for this software timer
    uint32_t    waitCycles;             // wait "cycles" for the software timer
    uint32_t    startCounter;           // last counter value when the SW timer started (C1)
    uint32_t    startRolloverCount;     // The number of hardware counter rollovers at the start of the this software timer
} OneShotSWTimer_t;
/*
 * This method starts the timer with different load values depending on the difficulty level
 */
void startOneShotTimer0(unsigned int LoadVal);

//This method initializes the timer
void InitTimer();

//This method returns the timer has expired or not
bool timer0Expired();

/*
 * This function ties the software period pointed by OST to hwtimer and sets its wait cycles
 */
void InitOneShotSWTimer(OneShotSWTimer_t* OST_p,
                        HWTimer_t* hwtimer_p,
                        uint32_t  waitTime);

/*
 * This function simply records the start time which is C1 from notes
 */
void StartOneShotSWTimer(OneShotSWTimer_t* OST_p);

/*
 * This function checks to see if the sw timer has expired, i.e. wait cycles has passed
 */
bool OneShotSWTimerExpired(OneShotSWTimer_t* OST_p);

void initHWTimer0();
void initHWTimer1();


#endif /* TIMER32_HAL_H_ */
