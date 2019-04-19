/*
 * Game_Play.c
 *
 *  Created on: Apr 11, 2019
 *      Author: abarh
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


#include "ButtonLED_HAL.h"
#include "graphics_HAL.h"
#include "Timer32_HAL.h"
#include "ADC_HAL.h"

#define RIGHT_THRESHOLD_1 15500
#define RIGHT_THRESHOLD_2 12000

#define LEFT_THRESHOLD_1  7000
#define LEFT_THRESHOLD_2  2000

#define BALL_TIME_STEP1 240000
#define BALL_TIME_STEP2 432000
#define BALL_TIME_STEP3 960000


typedef enum {game_display, throw_mode,roll_mode, move, direction, venti, grande} game_features;
typedef enum {Center, left5, left10, left15, Right5, Right10, Right15} angle;
typedef enum {right, not_right} joystick_position_r;
typedef enum {left, not_left} joystick_position_l;
typedef enum {up, not_up} joystick_position_u;


button_t BoosterS1 = {GPIO_PORT_P5, GPIO_PIN1, Stable_R, RELEASED_STATE, TIMER32_0_BASE};
button_t BoosterS2 = {GPIO_PORT_P3, GPIO_PIN5, Stable_R, RELEASED_STATE, TIMER32_1_BASE};
button_t Button_JoyStick = {GPIO_PORT_P4, GPIO_PIN1, Stable_R, RELEASED_STATE, TIMER32_1_BASE};


char JoyStick_pressed1();
bool IsJoystickPushedtoRight_debounced(unsigned Vx);
bool IsJoystickPushedtoLeft_debounced(unsigned vx);

bool IsJoystickPushedtoRight5_debounced(unsigned vx);
bool IsJoystickPushedtoRight10_debounced(unsigned vx);
bool IsJoystickPushedtoRight15_debounced(unsigned vx);

bool IsJoystickPushedtoLeft5_debounced(unsigned vx);
bool IsJoystickPushedtoLeft10_debounced(unsigned vx);
bool IsJoystickPushedtoLeft15_debounced(unsigned vx);

bool IsjoyStickPushedUp1(unsigned vy);
bool IsjoyStickPushedUp2(unsigned vy);

bool Bowling_Alley(Graphics_Context *g_sContext_p,int score[3])
{
    static bool inint = true;
    static unsigned vx, vy;
    static int values = 0;
    static int before_value = 0;
    static int count = 0;
    static int position = 55;
    static game_features mode = game_display;
    static angle trajectory = Center;
    static int roll_count = 0;
    static bool return_value = false;
    static int i = -1;
    static int scores[10];
    int LoadValue = BALL_TIME_STEP3;

    if(inint)
    {
        values = 0;
        before_value = 0;
        count = 0;
        position = 55;
        mode = game_display;
        trajectory = Center;
        roll_count = 0;
        return_value = false;
        i = -1;
        inint = false;
    }

    bool game_over = false;
    static int hit = 0;

    bool boosterS1 = false;
    bool boosterS2 = false;

    bool joyStickPushedUp1 = false;
    bool joyStickPushedUp2 = false;
    bool JoyStickPressed = false;

    bool joyStickPushedtoRight = false;
    bool joyStickPushedtoLeft = false;

    bool JoyStickNotPushed = true;

    bool joyStickPushedtoRight5 = false;
    bool joyStickPushedtoRight10 = false;
    bool joyStickPushedtoRight15 = false;

    bool joyStickPushedtoLeft5 = false;
    bool joyStickPushedtoLeft10 = false;
    bool joyStickPushedtoLeft15 = false;

    count = count + 1;

    getSampleJoyStick(&vx, &vy);

    boosterS1 = ButtonPushed(&BoosterS1);
    boosterS2 = ButtonPushed(&BoosterS2);

    JoyStickPressed = ButtonPushed(&Button_JoyStick);

    if(count == 5)
    {
        values = random_ball(g_sContext_p, vx, vy);
        count = 0;
    }

    switch(mode)
    {
    case game_display:
        display_game(g_sContext_p, score, position);
        Graphics_fillCircle(g_sContext_p, values+40, 5, 2);
        before_value = values;
        mode = throw_mode;
        break;
    case throw_mode:
        display_game(g_sContext_p, score, position);
        if(i == 0)
        {
            for(i = 1; i<=roll_count;i++)
            {
                display_score(g_sContext_p, i, scores);
            }
        }
        i = -1;
        if(boosterS1)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(g_sContext_p,position, 104, 1);
            Graphics_fillCircle(g_sContext_p,position, 107, 1);
            Graphics_fillCircle(g_sContext_p,position, 102, 1);
            Graphics_drawLineV(g_sContext_p, position, 102, 120);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            mode = move;
        }
        else if(boosterS2)
        {
            mode = direction;
        }
        else if(JoyStickPressed | return_value)
        {
            mode = roll_mode;
            roll_count = roll_count + 1;
            if(roll_count == 11)
            {
                game_over = true;
                inint = true;
            }
        }
        if(vy > 7900 && vy < 9000)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLineV(g_sContext_p, position, 103, 111);
            Graphics_fillCircle(g_sContext_p,position, 102, 1);
            Graphics_drawLineV(g_sContext_p, position, 105, 111);
            Graphics_fillCircle(g_sContext_p,position, 104, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            Graphics_drawLineV(g_sContext_p, position, 108, 116);
            Graphics_fillCircle(g_sContext_p,position, 107, 1);
        }
        else if(vy>= 9000 && vy< 12000)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLineV(g_sContext_p, position, 108, 111);
            Graphics_fillCircle(g_sContext_p,position, 107, 1);
            Graphics_drawLineV(g_sContext_p, position, 103, 111);
            Graphics_fillCircle(g_sContext_p,position, 102, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            Graphics_drawLineV(g_sContext_p, position, 105, 111);
            Graphics_fillCircle(g_sContext_p,position, 104, 1);
            LoadValue = BALL_TIME_STEP2;
        }
        else if(vy>= 12000)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLineV(g_sContext_p, position, 108, 111);
            Graphics_fillCircle(g_sContext_p,position, 107, 1);
            Graphics_drawLineV(g_sContext_p, position, 105, 111);
            Graphics_fillCircle(g_sContext_p,position, 104, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            Graphics_drawLineV(g_sContext_p, position, 103, 111);
            Graphics_fillCircle(g_sContext_p,position, 102, 1);
            LoadValue = BALL_TIME_STEP1;

        }
        break;
    case roll_mode:
        return_value = roll_ball(g_sContext_p, position, before_value, &hit, LoadValue);
        if(return_value ==  false)
        {
            display_game(g_sContext_p, score, position);
            score_points(roll_count, scores, hit);
            i = 0;
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(g_sContext_p, before_value+40, 5, 2);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            Graphics_fillCircle(g_sContext_p, values+40, 5, 2);
            before_value = values;
            hit = 0;
            turnOff_BoosterpackLED_red();
            turnOff_BoosterpackLED_green();
            mode = throw_mode;
        }
        break;
    case move:
        joyStickPushedtoRight = IsJoystickPushedtoRight_debounced(vx);
        joyStickPushedtoLeft = IsJoystickPushedtoLeft_debounced(vx);
        position = Move_Ball(g_sContext_p, joyStickPushedtoLeft,joyStickPushedtoRight);
        if(boosterS1)
        {
            display_Empty(g_sContext_p);
            i = 0;
            mode = throw_mode;
            Graphics_fillCircle(g_sContext_p, before_value+40, 5, 2);
        }
        break;
    case direction:
        joyStickPushedtoRight5 = IsJoystickPushedtoRight5_debounced(vx);
        joyStickPushedtoRight10 = IsJoystickPushedtoRight10_debounced(vx);
        joyStickPushedtoRight15 = IsJoystickPushedtoRight15_debounced(vx);
        joyStickPushedtoLeft5 = IsJoystickPushedtoLeft5_debounced(vx);
        joyStickPushedtoLeft10 = IsJoystickPushedtoLeft10_debounced(vx);
        joyStickPushedtoLeft15 = IsJoystickPushedtoLeft15_debounced(vx);

        if( vx > 8000 && vx <7900)
        {
            JoyStickNotPushed  = false;
        }
        else
        {
            JoyStickNotPushed = true;
        }

        if(JoyStickNotPushed)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);

        }
        if(joyStickPushedtoLeft5)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 7);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position+20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            JoyStickNotPushed  = false;

        }
        if(joyStickPushedtoLeft10)
        {
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 7);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position+20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position-20, 4);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            JoyStickNotPushed  = false;
        }
        if(joyStickPushedtoLeft15)
        {
            display_game(g_sContext_p, score,position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position+20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            JoyStickNotPushed  = false;
        }
        if(joyStickPushedtoRight5)
        {
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 7);
            Graphics_drawLine(g_sContext_p, position, 115, position+20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(joyStickPushedtoRight10)
        {
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 7);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position+20, 4);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(joyStickPushedtoRight15)
        {
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position-20, 4);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 7);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_drawLine(g_sContext_p, position, 115, position+20, 4);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            display_game(g_sContext_p, score, position);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(boosterS2)
        {
            display_Empty(g_sContext_p);
            mode = throw_mode;
            Graphics_fillCircle(g_sContext_p, values+40, 5, 2);
        }
        break;
    case venti:
        turnOn_BoosterpackLED_blue();
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(g_sContext_p, position, 107, 1);
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        Graphics_drawLineV(g_sContext_p, position, 106, 114);
        Graphics_fillCircle(g_sContext_p, position, 105, 1);
        if(joyStickPushedUp1 == false)
        {
            display_Empty(g_sContext_p);
            mode = throw_mode;
            Graphics_fillCircle(g_sContext_p, values+40, 5, 2);
            i = 0;
        }
        break;
    case grande:
        break;
    }
    return game_over;
}

bool IsjoyStickPushedUp1(unsigned vy)
{
    static bool isPushed = false;
    static joystick_position_u position = not_up;
    switch(position)
    {
    case up:
        if(vy > 13000 && vy < 8500)
        {
            position = not_up;
        }
        isPushed = false;
        break;
    case not_up:
        if(vy<= 13000 && vy>9000)
        {
            position = up;
            isPushed = true;
        }
        break;
    }
   return isPushed;
}
bool IsjoyStickPushedUp2(unsigned vy)
{
    return false;
}
bool IsJoystickPushedtoRight5_debounced(unsigned vx)
{
    static bool isPushed = false;
    static joystick_position_r position = not_right;
    switch(position)
    {
        case right:
            if(vx <= 8500)
            {
                position = not_right;
            }
            isPushed = false;
            break;
        case not_right:
            if(vx >9000 && vx < 11000)
            {
                isPushed = true;
                position = right;
            }
            break;
    }
    return isPushed;
}
bool IsJoystickPushedtoRight10_debounced(unsigned vx)
{
    static bool isPushed = false;
    static joystick_position_r position = not_right;
    switch(position)
    {
        case right:
            if(vx <= 11500)
            {
                position = not_right;
            }
            isPushed = false;
            break;
        case not_right:
            if(vx >12000 && vx < 13500)
            {
                isPushed = true;
                position = right;
            }
            break;
    }
    return isPushed;
}
bool IsJoystickPushedtoRight15_debounced(unsigned vx)
{
    static bool isPushed = false;
    static joystick_position_r position = not_right;
    switch(position)
    {
        case right:
            if(vx <= 14500)
            {
                position = not_right;
            }
            isPushed = false;
            break;
        case not_right:
            if(vx >14500)
            {
                isPushed = true;
                position = right;
            }
            break;
    }
    return isPushed;
}
bool IsJoystickPushedtoLeft5_debounced(unsigned vx)
{
    static bool isPushed = false;
    static joystick_position_l position = not_left;
    switch(position)
    {
    case left:
        if(vx <= 6000)
        {
            position = not_left;
        }
        isPushed = false;
        break;
    case not_left:
        if(vx> 6000 && vx< 7960)
        {
            isPushed = true;
            position = left;
        }
        break;
    }
    return isPushed;
}
bool IsJoystickPushedtoLeft10_debounced(unsigned vx)
{
    static bool isPushed = false;
    static joystick_position_l position = not_left;
    switch(position)
    {
    case left:
        if(vx <= 3500)
        {
            position = not_left;
        }
        isPushed = false;
        break;
    case not_left:
        if(vx > 3500 && vx < 5500)
        {
            isPushed = true;
            position = left;
        }
        break;
    }
    return isPushed;
}
bool IsJoystickPushedtoLeft15_debounced(unsigned vx)
{
    static bool isPushed = false;
    static joystick_position_l position = not_left;
    switch(position)
    {
    case left:
        if(vx <= 84)
        {
            position = not_left;
        }
        isPushed = false;
        break;
    case not_left:
        if(vx> 84 && vx< 3000)
        {
            isPushed = true;
            position = left;
        }
        break;
    }
    return isPushed;
}


bool IsJoystickPushedtoLeft_debounced(unsigned vx)
{
    static bool isPushed = false;

    static joystick_position_l position = not_left;

    switch(position)
    {
    case left:
        if(vx >= LEFT_THRESHOLD_1)
        {
            position = not_left;
        }
        isPushed = false;
        break;
    case not_left:
        if(vx < LEFT_THRESHOLD_2)
        {
            isPushed = true;
            position = left;
        }
        break;
    }
    return isPushed;
}

bool IsJoystickPushedtoRight_debounced(unsigned Vx)
{
    static bool isPushed = false;

    static joystick_position_r position = not_right;

    switch(position)
    {
    case right:
        if(Vx <= RIGHT_THRESHOLD_2)
        {
            position = not_right;
        }
        isPushed = false;
        break;
    case not_right:
        if(Vx > RIGHT_THRESHOLD_1)
        {
            isPushed = true;
            position = right;
        }
        break;
    }
   return isPushed;
}


char JoyStick_pressed1()
{
    return ((GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1)) == 0);

}

