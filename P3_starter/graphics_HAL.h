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
void display_How_To_Play(Graphics_Context *g_sContext_p);
void display_Empty(Graphics_Context *g_sContext_p);
void display_High_Score(Graphics_Context *g_sContext_p, int score[3]);
void display_game(Graphics_Context *g_sContext_p, int score[3], int position);

int Move_Ball(Graphics_Context *g_sContext_p, bool moveToLeft, bool moveToRight);
bool roll_ball(Graphics_Context *g_sContext_p, int position, int before_value, int score[3]);

int random_ball(Graphics_Context *g_sContext_p, unsigned int x, unsigned int y);
void display_random_ball(Graphics_Context *g_sContext_p, int *values, int *before_values);
void score_points(Graphics_Context *g_sContext_p,int score[3], bool hit);



#endif /* GRAPHICS_HAL_H_ */
