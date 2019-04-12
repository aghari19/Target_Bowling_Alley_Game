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

typedef enum {game_display, throw_mode, move, direction, speed1, speed2} game_features;
typedef enum {right, not_right} joystick_position_r;
typedef enum {left, not_left} joystick_position_l;

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

void Bowling_Alley(Graphics_Context *g_sContext_p,int score[3])
{
    static unsigned vx, vy;
    static int position = 55;
    static game_features mode = game_display;

    bool return_value = false;

    bool boosterS1 = false;
    bool boosterS2 = false;

    bool joyStickPushedUp1 = false;
    bool joyStickPushedUp2 = false;

    bool joyStickPushedtoRight = false;
    bool joyStickPushedtoLeft = false;

    bool joyStickPushedtoRight5 = false;
    bool joyStickPushedtoRight10 = false;
    bool joyStickPushedtoRight15 = false;

    bool joyStickPushedtoLeft5 = false;
    bool joyStickPushedtoLeft10 = false;
    bool joyStickPushedtoLeft15 = false;


    getSampleJoyStick(&vx, &vy);

    boosterS1 = ButtonPushed(&BoosterS1);
    boosterS2 = ButtonPushed(&BoosterS2);

    switch(mode)
    {
    case game_display:
        display_game(g_sContext_p, score);
        mode = throw_mode;
        break;
    case throw_mode:
        if(boosterS1)
        {
            mode = move;
        }
        else if(boosterS2)
        {
            mode = direction;
        }
        else if(joyStickPushedUp1)
        {
            mode = speed1;
        }
        else if(joyStickPushedUp2)
        {
            mode = speed2;
        }
        else if(JoyStick_pressed1() | return_value)
        {
            return_value = roll_ball(g_sContext_p, position);
        }
        break;
    case move:
        turnOn_BoosterpackLED_green();
        joyStickPushedtoRight = IsJoystickPushedtoRight_debounced(vx);
        joyStickPushedtoLeft = IsJoystickPushedtoLeft_debounced(vx);
        position = Move_Ball(g_sContext_p, joyStickPushedtoLeft,joyStickPushedtoRight);
        if(boosterS1)
        {
            mode = throw_mode;
            Graphics_drawLineV(g_sContext_p, position, 108, 116);
            Graphics_fillCircle(g_sContext_p, position, 107, 1);
        }
        break;
    case direction:
        joyStickPushedtoRight5 = IsJoystickPushedtoRight5_debounced(vx);
        joyStickPushedtoRight10 = IsJoystickPushedtoRight10_debounced(vx);
        joyStickPushedtoRight15 = IsJoystickPushedtoRight15_debounced(vx);
        joyStickPushedtoLeft5 = IsJoystickPushedtoLeft5_debounced(vx);
        joyStickPushedtoLeft10 = IsJoystickPushedtoLeft10_debounced(vx);
        joyStickPushedtoLeft15 = IsJoystickPushedtoLeft15_debounced(vx);

        if(joyStickPushedtoLeft5)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position+60, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);

        }
        if(joyStickPushedtoLeft10)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position-10, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 4);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(joyStickPushedtoLeft15)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position-30, 4);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position-60, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(joyStickPushedtoRight5)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(joyStickPushedtoRight10)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position+10, 1);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 4);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(joyStickPushedtoRight15)
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_drawLine(g_sContext_p, position, 115, position+30, 4);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLUE);
            Graphics_drawLine(g_sContext_p, position, 115, position+60, 7);
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        }
        if(boosterS2)
        {
            mode = throw_mode;
        }
        break;
    case speed1:
        break;
    case speed2:
        break;
    }

}
bool IsJoystickPushedtoRight5_debounced(unsigned vx)
{
    static bool isPushed = false;
    static joystick_position_r position = not_right;
    switch(position)
    {
        case right:
            if(vx <= 7000)
            {
                position = not_right;
            }
            isPushed = false;
            break;
        case not_right:
            if(vx >8000 && vx < 9500)
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
            if(vx <= 10000)
            {
                position = not_right;
            }
            isPushed = false;
            break;
        case not_right:
            if(vx >10500 && vx < 13000)
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
            if(vx <= 13500)
            {
                position = not_right;
            }
            isPushed = false;
            break;
        case not_right:
            if(vx >14000)
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
        if(vx <= 4000)
        {
            position = not_left;
        }
        isPushed = false;
        break;
    case not_left:
        if(vx>= 4000 && vx< 5000)
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
        if(vx <= 2000)
        {
            position = not_left;
        }
        isPushed = false;
        break;
    case not_left:
        if(vx>= 2000 && vx< 4000)
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
        if(vx <= 500)
        {
            position = not_left;
        }
        isPushed = false;
        break;
    case not_left:
        if(vx>= 500 && vx< 2000)
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

