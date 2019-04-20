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
#include "sound.h"

#define UP1 15000
#define UP2 13000

#define DOWN1 2000
#define DOWN2 500

typedef enum {Main, Menu, how_to_play, highscore, main_game, game_over} game_options;
typedef enum {how_to_play1, how_to_play2, how_to_play3} ins;
typedef enum {up, not_up} joystick_position_up;
typedef enum {down, not_down} joystick_position_down;

song_note_t note1 = {note_f4, 100};

button_t BoostS1 = {GPIO_PORT_P5, GPIO_PIN1, Stable_R, RELEASED_STATE, TIMER32_0_BASE};
button_t BoostS2 = {GPIO_PORT_P3, GPIO_PIN5, Stable_R, RELEASED_STATE, TIMER32_1_BASE};
button_t JoyStick_Button = {GPIO_PORT_P4, GPIO_PIN1, Stable_R, RELEASED_STATE, TIMER32_1_BASE};

bool IsJoystickDown_debounced(unsigned Vy);
bool IsJoystickUp_debounced(unsigned Vy);
bool Bowling_Alley(Graphics_Context *g_sContext_p,int score[3]);
void drawGameOver(Graphics_Context *g_sContext_p,Graphics_Image *Image);

extern Graphics_Image Jon_Bunting8BPP_UNCOMP;

void game()
{
    static unsigned vx, vy;
    static Graphics_Context g_sContext;
    static game_options game = Main;
    static ins play = how_to_play1;
    static int menu_location = 55;
    static int i = 0;
    static int score[3] = {0,0,0};
    static bool isMenu = false;
    static bool game_status;
    static bool playing;
    static bool check1 = false;
    static bool check2 = false;

    bool joystick_button_pressed = false;
    bool Booster1_Pressed  = false;
    bool Booster2_Pressed = false;
    bool joyStickPushedUp = false;
    bool joyStickPushedDown = false;

    joystick_button_pressed = ButtonPushed(&JoyStick_Button);
    Booster1_Pressed = ButtonPushed(&BoostS1);
    Booster2_Pressed = ButtonPushed(&BoostS2);
    getSampleJoyStick(&vx, &vy);

    switch (game)
    {
        case Main:
            InitGraphics (&g_sContext);
            game = Menu;
            isMenu = true;
            break;
        case Menu:
            displayMenu(&g_sContext, &i);
            if((joystick_button_pressed) && (menu_location == 55))
            {
                display_Empty(&g_sContext);
                game = main_game;
                isMenu = false;
            }
            else if((joystick_button_pressed) && (menu_location == 75))
            {
                display_Empty(&g_sContext);
                game = highscore;
                isMenu = false;
            }
            else if((joystick_button_pressed) && (menu_location == 95))
            {
                display_Empty(&g_sContext);
                game = how_to_play;
                play = how_to_play1;
                isMenu = false;
            }
            break;
        case how_to_play:
            switch(play)
            {
                case how_to_play1:
                    display_How_To_Play(&g_sContext);
                    if(Booster1_Pressed)
                    {
                        display_Empty(&g_sContext);
                        play = how_to_play2;
                    }
                    if(Booster2_Pressed)
                    {
                        display_Empty(&g_sContext);
                        play = how_to_play3;
                    }
                    if(joystick_button_pressed)
                    {
                        display_Empty(&g_sContext);
                        Graphics_fillCircle(&g_sContext, 20,menu_location, 4);
                        game = Menu;
                        isMenu = true;
                    }
                break;
                case how_to_play2:
                    display_How_To_PlayS1(&g_sContext);
                    if(Booster2_Pressed)
                    {
                        display_Empty(&g_sContext);
                        play = how_to_play3;
                    }
                    if(Booster1_Pressed)
                    {
                        display_Empty(&g_sContext);
                        play = how_to_play1;
                    }
                    if(joystick_button_pressed)
                    {
                        display_Empty(&g_sContext);
                        Graphics_fillCircle(&g_sContext, 20,menu_location, 4);
                        game = Menu;
                        isMenu = true;
                    }
                break;
                case how_to_play3:
                    display_How_To_PlayS2(&g_sContext);
                    if(Booster1_Pressed)
                    {
                        display_Empty(&g_sContext);
                        play = how_to_play2;
                    }
                    if(Booster2_Pressed)
                    {
                        display_Empty(&g_sContext);
                        play = how_to_play1;
                    }
                    if(joystick_button_pressed)
                    {
                        display_Empty(&g_sContext);
                        Graphics_fillCircle(&g_sContext, 20,menu_location, 4);
                        game = Menu;
                        isMenu = true;
                    }
                break;
            }
            break;
        case highscore:
            display_High_Score(&g_sContext,score);
            if(joystick_button_pressed)
            {
                turnOff_LaunchpadLED1();
                display_Empty(&g_sContext);
                Graphics_fillCircle(&g_sContext, 20,menu_location, 4);
                game = Menu;
                isMenu = true;
            }
            break;
        case main_game:
            game_status = Bowling_Alley(&g_sContext, score);
            if(game_status == true)
            {
                game = game_over;
            }
            break;
        case game_over:
            turnOn_BoosterpackLED_red();
            drawGameOver(&g_sContext, &Jon_Bunting8BPP_UNCOMP);
            //playing = PlayNote_nonblocking(note1);
                if(joystick_button_pressed)
                {
                    display_Empty(&g_sContext);
                    Graphics_fillCircle(&g_sContext, 20,menu_location, 4);
                    game = Menu;
                    isMenu = true;
                }
            break;
    }

    joyStickPushedDown = IsJoystickDown_debounced(vy);
    joyStickPushedUp = IsJoystickUp_debounced(vy);
    if((joyStickPushedDown | joyStickPushedUp) && isMenu)
    {
        menu_location = MoveCircle(&g_sContext, joyStickPushedUp,joyStickPushedDown);
    }
}

void drawGameOver(Graphics_Context *g_sContext_p,Graphics_Image *Image)
{
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_RED);
    Graphics_drawImage(g_sContext_p, Image, 0, 0);
    int8_t Over[12] = "!GAME OVER!";
    Graphics_drawString(g_sContext_p, Over, -1, 25, 100, false);
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
