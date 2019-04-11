/*
 * Timer32_HAL.h
 *
 */

#ifndef TIMER32_HAL_H_
#define TIMER32_HAL_H_

typedef struct
{
    unsigned int tenth;
    unsigned int sec;
    unsigned int min;
    unsigned int hour;
} time_t;
/*
 * This method starts the timer with different load values depending on the difficulty level
 */
void startOneShotTimer0(unsigned int LoadVal);
void startOneShotTimer1(unsigned int LoadVal);

//This method initializes the timer
void InitTimer();

//This method returns the timer has expired or not
bool timer0Expired();

//This method taken in a number in this case time and converts it into a string
void makeTimeToString(time_t time, int8_t *string);

//This method increses the timers once the timer starts at very time the loop is encountered
void increaseTime(time_t *time_p);

#endif /* TIMER32_HAL_H_ */
