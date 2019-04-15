/*
 * graphics_HAL.c
 *
 *  Created on: Apr 9, 2019
 *      Author: abarh
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

#include "ButtonLED_HAL.h"
#include "ADC_HAL.h"
#include "graphics_HAL.h"
#include "Timer32_HAL.h"

HWTimer_t timer0, timer1;

#define BALL_Y_STEP 12
#define BALL_TIME_STEP 3840000

int random_ball(Graphics_Context *g_sContext_p, unsigned vx, unsigned vy)
{
    int x = 1;
    int y = 1;

    static int i = 0;
    static int values = 0;

    x = vx & x;
    y = vy & y;
    if(i == 5)
    {
        i = 0;
        values = 0;
    }
    if(i != 5)
    {
        values = values << 1 | (x ^ y);
        i++;
    }

    return values;
}

void make_5digit_NumString(unsigned int num, char *string)
{
    string[0]= (        num  / 10000) +'0';
    string[1]= ((num%10000) / 1000) +'0';
    string[2]= ((num%1000) / 100) +'0';
    string[3]= ((num%100) / 10) +'0';
    string[4]= ((num%10) / 1) +'0';
    string[5]= 0;
}

void make_2digit_NumString(unsigned int num, char *string)
{
    string[0]= ((num%100) / 10) +'0';
    string[1]= ((num%10) / 1) +'0';
}

void drawXY(Graphics_Context *g_sContext_p, unsigned int x, unsigned int y)
{
    char string[6];

    Graphics_drawString(g_sContext_p,(int8_t*) "x=", -1, 10, 5, true);
    make_5digit_NumString(x, string);
    Graphics_drawString(g_sContext_p,(int8_t*) string, -1, 30, 5, true);

    Graphics_drawString(g_sContext_p,(int8_t*) "y=", -1, 10, 15, true);
    make_5digit_NumString(y, string);
    Graphics_drawString(g_sContext_p,(int8_t*) string, -1, 30, 15, true);
}

void draw_Base(Graphics_Context *g_sContext_p)
{
    Graphics_Rectangle R;
    R.xMin = 0;
    R.xMax = 127;
    R.yMin = 32;
    R.yMax = 96;

    Graphics_drawRectangle(g_sContext_p, &R);
    Graphics_fillCircle(g_sContext_p, 63, 63, 10);

}

void make_3digit_NumString(unsigned int num, char *string)
{
    string[0]= (num/100)+'0';
    string[1]= ((num%100) / 10) + '0';
    string[2]= (num%10)+'0';
    string[3] =0;

}

bool roll_ball(Graphics_Context *g_sContext_p, int position)
{
    static int y = 115;
    static bool moveBallUp = true;
    static bool init = true;
    if (init)
    {
        moveBallUp = true;
        y = 115;
        startOneShotTimer0(BALL_TIME_STEP);
        init = false;
    }

    if(timer0Expired())
    {
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(g_sContext_p, position, y, 3);

        startOneShotTimer0(BALL_TIME_STEP);
        if (moveBallUp)
        {
            y = y - BALL_Y_STEP;
        }
        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        Graphics_fillCircle(g_sContext_p, position, y, 3);
        if (y <= 0)
        {
            init = true;
            moveBallUp = false;
        }
    }
    return moveBallUp;
}

int Move_Ball(Graphics_Context *g_sContext_p, bool moveToLeft, bool moveToRight)
{
    static unsigned int x = 55;

    if ((moveToLeft && (x>40)) || (moveToRight && (x<70)))
        {
            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
            Graphics_fillCircle(g_sContext_p, x,115, 3);
            Graphics_drawLineV(g_sContext_p, x, 108, 116);
            Graphics_fillCircle(g_sContext_p, x, 107, 1);


            if (moveToLeft)
                x = x-5;
            if(moveToRight)
                x = x+5;

            Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
            Graphics_fillCircle(g_sContext_p, x,115, 3);
        }
    return x;
}


int MoveCircle(Graphics_Context *g_sContext_p, bool joyStickPushedUp, bool joyStickPushedDown)
{
    static unsigned int y = 55;

    if ((joyStickPushedUp && (y>55)) || (joyStickPushedDown && (y<90)))
    {

        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(g_sContext_p, 20, y, 4);

        if (joyStickPushedDown)
            y = y+20;
        else if(joyStickPushedUp)
            y = y-20;


        Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
        Graphics_fillCircle(g_sContext_p, 20, y, 4);

    }
    return y;
}

void draw_One_Zero(Graphics_Context *g_sContext_p, unsigned int vx, unsigned int vy)
{
    unsigned x = 1;
    x = vx & x;

    unsigned y = 1;
    y = vy & y;

    unsigned int value = x ^ y;
    static int ones = 0;
    static int zero = 0;

    char string1[6];
    char string2[6];

    if(value == 1)
    {
        ones++;
    }
    else if(value == 0)
    {
        zero++;
    }

    Graphics_drawString(g_sContext_p, (int8_t*) "ones= ", -1, 10, 100, true);

    make_5digit_NumString(ones, string1);
    Graphics_drawString(g_sContext_p, (int8_t*) string1, -1, 70, 100, true);

    Graphics_drawString(g_sContext_p, (int8_t*) "zeros= ", -1, 10, 110, true);
    make_5digit_NumString(zero, string2);
    Graphics_drawString(g_sContext_p, (int8_t*) string2, -1, 70, 110, true);

}

void InitFonts()
{
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
}

void InitGraphics(Graphics_Context *g_sContext_p)
{

    Graphics_initContext(g_sContext_p,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontCm12);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);
}

void displayMenu(Graphics_Context *g_sContext_p, int *i)
{
    if(*i == 0)
    {
        turnOff_BoosterpackLED_red();
        Graphics_fillCircle(g_sContext_p, 20, 55, 4);
        *i = 1;
    }
    Graphics_drawString(g_sContext_p,(int8_t*) "PLAY", -1, 30, 50, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "HIGHSCORE", -1, 30, 70, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "HOW TO PLAY", -1, 30, 90, true);
}

void display_Empty(Graphics_Context *g_sContext_p)
{

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);

    Graphics_Rectangle Rec = {0,0, 128, 128};
    Graphics_fillRectangle(g_sContext_p, &Rec);

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
}

void display_game(Graphics_Context *g_sContext_p, int score[3], int ball_position)
{
    //static int ball_position = 55;
    Graphics_Rectangle Rec = {80,5, 120, 120};

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);

    Graphics_drawRectangle(g_sContext_p, &Rec);

    Graphics_drawLineV(g_sContext_p, 100, 5, 120);
    Graphics_drawLineH(g_sContext_p, 80, 120, 28);
    Graphics_drawLineH(g_sContext_p, 80, 120, 51);
    Graphics_drawLineH(g_sContext_p, 80, 120, 74);
    Graphics_drawLineH(g_sContext_p, 80, 120, 97);

    Graphics_drawLineV(g_sContext_p, 75, 5, 120);
    Graphics_drawLineV(g_sContext_p, 35, 5, 120);

    Graphics_fillCircle(g_sContext_p, ball_position, 115, 3);
    Graphics_drawLineV(g_sContext_p, ball_position, 108, 116);
    Graphics_fillCircle(g_sContext_p, ball_position, 107, 1);
}

void display_High_Score(Graphics_Context *g_sContext_p, int score[3])
{
    char string1[3];
    char string2[3];
    char string3[3];

    Graphics_drawString(g_sContext_p,(int8_t*) "HIGH SCORES", -1, 25, 10, true);

    unsigned int num = score[0];
    make_2digit_NumString(num, string1);

    Graphics_drawString(g_sContext_p,(int8_t*) string1, -1, 5, 40, true);

    num = score[1];
    make_2digit_NumString(num, string2);

    Graphics_drawString(g_sContext_p,(int8_t*) string2, -1, 5, 60, true);

    num = score[2];
    make_2digit_NumString(num, string3);

    Graphics_drawString(g_sContext_p,(int8_t*) string3, -1, 5, 80, true);
}

void display_How_To_Play(Graphics_Context *g_sContext_p)
{
    Graphics_drawString(g_sContext_p,(int8_t*) "How to Play:", -1, 24, 5, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "The game begins in", -1, 5, 20, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "the Throwing Mode:", -1, 5, 35, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "Use the JoyStick Up/", -1, 5, 50, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "Down to select High,", -1, 5, 65, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "Medium and Low", -1, 5, 80, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "S1: Throw & position", -1, 5, 95, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "S2: Throw to direct", -1, 5, 110, true);
}
