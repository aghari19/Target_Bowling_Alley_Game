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
void drawXY(Graphics_Context *g_sContext_p, unsigned int x, unsigned int y);
void draw_One_Zero(Graphics_Context *g_sContext_p, unsigned int vx, unsigned int vy);

//this method converts a two digit number to a string
void make_2digit_NumString(unsigned int num, char *string);

//This method initializes the graphics.
void InitGraphics(Graphics_Context *g_sContext_p);
//This method draws the menu screen
void displayMenu(Graphics_Context *g_sContext_p, int *i);

//This method displays the basic how to play screen
void display_How_To_Play(Graphics_Context *g_sContext_p);

//this method draws the how to play in position mode
void display_How_To_PlayS1(Graphics_Context *g_sContext_p);

//this method draws the how to play in direction mode
void display_How_To_PlayS2(Graphics_Context *g_sContext_p);

//this method prints an empty black rectangle and is called before each new drawing.
void display_Empty(Graphics_Context *g_sContext_p);

//this method displays the high score
void display_High_Score(Graphics_Context *g_sContext_p, int score[3]);

//this method draws the basic bowling alley with an empty score board
void display_game(Graphics_Context *g_sContext_p, int score[3], int position);

//This ball is called when in positioning mode and the joystick is toggeled to the left or the right. move the bowling ball in the direction
//and returns the position of the ball
int Move_Ball(Graphics_Context *g_sContext_p, bool moveToLeft, bool moveToRight);

//this method is to roll the ball in a straight line.
bool roll_ball(Graphics_Context *g_sContext_p, int position, int before_value, int *hit, int LoadValue);

//this method is to roll the ball at an angle of 5 degrees to the left.
bool roll_ball5(Graphics_Context *g_sContext_p, int position, int before_value, int *hit, int LoadValue);

//this method is to roll the ball at an angle of 10 degrees to the left.
bool roll_ball10(Graphics_Context *g_sContext_p, int position, int before_value, int *hit, int LoadValue);

//this method is to roll the ball at an angle of 15 degrees to the left.
bool roll_ball15(Graphics_Context *g_sContext_p, int position, int before_value, int *hit, int LoadValue);

//this method is to roll the ball at an angle of 5 degrees to the right.
bool roll_ballR5(Graphics_Context *g_sContext_p, int position, int before_value, int *hit, int LoadValue);

//this method is to roll the ball at an angle of 10 degrees to the right.
bool roll_ballR10(Graphics_Context *g_sContext_p, int position, int before_value, int *hit, int LoadValue);

//this method is to roll the ball at an angle of 15 degrees to the right.
bool roll_ballR15(Graphics_Context *g_sContext_p, int position, int before_value, int *hit, int LoadValue);

//this method generates a random ball.
int random_ball(Graphics_Context *g_sContext_p, unsigned int x, unsigned int y);

//this method display the random ball that is generated in the previous method
void display_random_ball(Graphics_Context *g_sContext_p, int *values, int *before_values);

//this method calculates the score of the player after every roll
void score_points(Graphics_Context *g_sContext_p, int roll_count, int score[10], int hit);

//this method displays the score of the player
void display_score(Graphics_Context *g_sContext_p,int roll_count, int score[10]);

#endif /* GRAPHICS_HAL_H_ */
