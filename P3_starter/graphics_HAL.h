/*
 * graphics_HAL.h
 *
 *  Created on: Apr 9, 2019
 *      Author: abarh
 */

#ifndef GRAPHICS_HAL_H_
#define GRAPHICS_HAL_H_

#include <ti/grlib/grlib.h>

void draw_Base(Graphics_Context *g_sContext_p);
int MoveCircle(Graphics_Context *g_sContext_p, bool moveToLeft, bool moveToRight);
void InitGraphics(Graphics_Context *g_sContext_p);
void drawXY(Graphics_Context *g_sContext_p, unsigned int x, unsigned int y);
void draw_One_Zero(Graphics_Context *g_sContext_p, unsigned int vx, unsigned int vy);
void displayMenu(Graphics_Context *g_sContext_p, int *i);
void display_How_To_Play(Graphics_Context *g_sContext_p, int menu_location);
void display_Empty(Graphics_Context *g_sContext_p);



#endif /* GRAPHICS_HAL_H_ */
