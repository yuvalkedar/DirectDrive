/*
Direct Drive LED Matrix

Author: Yuval Kedar, KD Tech.
Platform: Arduino Pro-mini
Date: Jan 2019
*/

#include <Arduino.h>
#include "fonts.h"

//rows are negative
//columns are positive
#define WIDTH (5)
#define HEIGHT (8)

#define COL_0  (9)
#define COL_1  (8)
#define COL_2  (7)
#define COL_3  (6)
#define COL_4  (5)
#define ROW_0  (10)
#define ROW_1  (11)
#define ROW_2  (12)
#define ROW_3  (13)
#define ROW_4  (14)
#define ROW_5  (15)
#define ROW_6  (16)
#define ROW_7  (17)

const uint8_t row[HEIGHT] = {ROW_0, ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7};
const uint8_t col[WIDTH] = {COL_0, COL_1, COL_2, COL_3, COL_4};

uint8_t pixel[HEIGHT][WIDTH];  //pixel[rows][columns]

const char message[] = "BANANA";

//////////////////////FUNCTIONS///////////////////////

void clear_screen(){
  for(uint8_t r = 0; r < HEIGHT; r++){
    for(uint8_t c = 0; c < WIDTH; c++){ //LOL c++
      pixel[r][c] = 0;
    }
  }
}

void set_pixel(uint8_t x, uint8_t y, bool state){ pixel[x][y] = state;}

void draw_pixel(){
  for(uint8_t r = 0; r < HEIGHT; r++){
    for(uint8_t i = 0; i < WIDTH; i++) digitalWrite(col[i], LOW); // turn previous col off
    digitalWrite(row[r], LOW);
    for(uint8_t c = 0; c < WIDTH; c++){ //LOL c++
      digitalWrite(col[c], pixel[r][c]);  // get the state of the current pixel
      delayMicroseconds(10);
    }
    digitalWrite(row[r], HIGH);
  }
}

void set_char(uint8_t pattern){
  for(uint8_t r = 0; r < HEIGHT; r++){
    for(uint8_t c = 0; c < WIDTH; c++){ //LOL c++
      pixel[r][c] = patterns[pattern][r][c];
    }
  }
}

void print_text(char *c){
  
}

void setup(){
  for(uint8_t r = 0; r < HEIGHT; r++){
    pinMode(row[r], OUTPUT);
    digitalWrite(row[r], HIGH); //turn all rows (negative) off.
  }

  for(uint8_t c = 0; c < WIDTH; c++){ //LOL c++
    pinMode(col[c], OUTPUT);
    digitalWrite(col[c], LOW); //turn all columns (positive) off.
  }

  clear_screen();
  set_pattern(pattern);
}

void loop(){
//  set_pixel(0, 0, 1);
//  set_pixel(1, 1, 1);
//  set_pixel(2, 2, 1);
//  set_pixel(3, 3, 1);
//  draw_pixel();

//pattern = ++pattern % numPatterns;
//draw_attern(pattern, 60);
draw_pixel();
}
