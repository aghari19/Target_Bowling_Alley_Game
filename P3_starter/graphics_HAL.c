/*
 * graphics_HAL.c
 *
 *  Created on: Apr 9, 2019
 *      Author: abarh
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

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
    string[1]= ((num%100) / 10) +'0';
    string[2]= ((num%10) / 1) +'0';
    string[3]= 0;
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
    Graphics_setFont(g_sContext_p, &g_sFontCmtt12);

    InitFonts();

    Graphics_clearDisplay(g_sContext_p);
}

void displayMenu(Graphics_Context *g_sContext_p, int *i)
{
    if(*i == 0)
    {
        Graphics_fillCircle(g_sContext_p, 20, 55, 4);
        *i = 1;
    }
    Graphics_drawString(g_sContext_p,(int8_t*) "PLAY", -1, 30, 50, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "HIGHSCORE", -1, 30, 70, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "HOW TO PLAY", -1, 30, 90, true);

    //extern Graphics_Image Menu8BPP_UNCOMP;
    //Graphics_drawImage(g_sContext_p, &Menu8BPP_UNCOMP, 0, 0);
}

void display_Empty(Graphics_Context *g_sContext_p)
{

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);

    Graphics_Rectangle Rec = {0,0, 128, 128};
    Graphics_fillRectangle(g_sContext_p, &Rec);

    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
}


void display_How_To_Play(Graphics_Context *g_sContext_p, int menu_location)
{
    /*Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(g_sContext_p,(int8_t*) "PLAY", -1, 30, 50, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "HIGHSCORE", -1, 30, 70, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "HOW TO PLAY", -1, 30, 90, true);
    Graphics_fillCircle(g_sContext_p, 20,menu_location , 4);*/

    extern Graphics_Image How_to_play8BPP_UNCOMP;
    Graphics_drawImage(g_sContext_p, &How_to_play8BPP_UNCOMP, 0, 0);

    /*Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_drawString(g_sContext_p,(int8_t*) "How to Play:", -1, 24, 10, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "The game begins", -1, 10, 20, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "in the Throwing", -1, 10, 30, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "Mode:Use the JoyStick Up/", -1, 10, 40, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "Down to select High,", -1, 10, 50, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "Medium and Low", -1, 10, 60, true);
    Graphics_drawString(g_sContext_p,(int8_t*) "S1: Throw & position the ball", -1, 10, 70, true);*/
    //Graphics_drawString(g_sContext_p,(int8_t*) "Medium and Low", -1, 10, 80, true);
    //Graphics_drawString(g_sContext_p,(int8_t*) "Medium and Low", -1, 10, 90, true);
}
