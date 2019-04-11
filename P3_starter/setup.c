/*
 * setup.c
 *
 *  Created on: Apr 10, 2019
 *      Author: abarh
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


#include "ButtonLED_HAL.h"
#include "graphics_HAL.h"
#include "Timer32_HAL.h"
#include "ADC_HAL.h"

#define UP1 15000
#define UP2 13000

#define DOWN1 2000
#define DOWN2 500

typedef enum {Main, Menu, how_to_play, highscore, main_game} game_options;
typedef enum {up, not_up} joystick_position_up;
typedef enum {down, not_down} joystick_position_down;

bool IsJoystickDown_debounced(unsigned Vy);
bool IsJoystickUp_debounced(unsigned Vy);

char JoyStick_pressed();


void game()
{
    static unsigned vx, vy;
    static Graphics_Context g_sContext;
    static game_options game = Main;
    static int menu_location = 55;
    static int i = 0;
    static int score[3] = {0,0,0};

    button_t LauchpadLeftButton = {GPIO_PORT_P1, GPIO_PIN1, Stable_R, RELEASED_STATE, TIMER32_0_BASE};
    button_t LauchpadRightButton = {GPIO_PORT_P1, GPIO_PIN4, Stable_R, RELEASED_STATE, TIMER32_1_BASE};

    getSampleJoyStick(&vx, &vy);

    bool joyStickPushedUp = false;
    bool joyStickPushedDown = false;

    switch (game)
    {
        case Main:
            InitGraphics (&g_sContext);
            game = Menu;
            break;
        case Menu:
            displayMenu(&g_sContext, &i);
            if(JoyStick_pressed() && menu_location == 55)
            {
                display_Empty(&g_sContext);
                game = main_game;
            }
            else if(JoyStick_pressed() && menu_location == 75)
            {
                display_Empty(&g_sContext);
                game = highscore;
            }
            else if(JoyStick_pressed() && menu_location == 95)
            {
                display_Empty(&g_sContext);
                game = how_to_play;
            }
            break;
        case how_to_play:
            display_How_To_Play(&g_sContext, menu_location);
            if(JoyStick_pressed() == true)
            {
                turnOn_BoosterpackLED_red();
                display_Empty(&g_sContext);
                menu_location = 55;
                i = 0;
                game = Menu;
            }
            break;
        case highscore:
            display_High_Score(&g_sContext,score);
            if(JoyStick_pressed() == true)
            {
                turnOn_BoosterpackLED_red();
                display_Empty(&g_sContext);
                menu_location = 55;
                i = 0;
                game = Menu;
            }
            break;
        case main_game:
            display_game(&g_sContext,score);
            break;
    }

    joyStickPushedDown = IsJoystickDown_debounced(vy);
    joyStickPushedUp = IsJoystickUp_debounced(vy);
    if(joyStickPushedDown | joyStickPushedUp)
    {
        menu_location = MoveCircle(&g_sContext, joyStickPushedUp,joyStickPushedDown);
    }
}

bool IsJoystickUp_debounced(unsigned Vy)
{
    static bool isPushed = false;
    static joystick_position_up position = not_up;

    switch(position)
    {
    case up:
        if(Vy <= UP2)
        {
            position = not_up;
        }
        isPushed = false;
        break;
    case not_up:
        if(Vy > UP1)
        {
            isPushed = true;
            position = up;
        }
        break;
    }
    return isPushed;
}

bool IsJoystickDown_debounced(unsigned Vy)
{
    static bool isPushed = false;

    static joystick_position_down position = not_down;

    switch(position)
    {
    case down:
        if(Vy >= DOWN1)
        {
            position = not_down;
        }
        isPushed = false;
        break;
    case not_down:
        if(Vy < DOWN2)
        {
            isPushed = true;
            position = down;
        }
        break;
    }
   return isPushed;
}

char JoyStick_pressed()
{
    return ((GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1)) == 0);

}

