/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains everything related to space junk 
for the Asteroid Apocalypse teensypewpew program.

Score is defined here.

*/

// Import .h files.
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

#include "cpu_speed.h"
#include "graphics.h"
#include "lcd.h"
#include "lcd_model.h"
#include "macros.h"
#include "usb_serial.h"
#include "cab202_adc.h"
#include "setup_teensy.h"
#include "starfighter.h"
#include "game_timer.h"

// Global variables.
#define DEFAULT_SCORE 0
static int score;

// Setup space junk.
void setup_space_junk() {
    // Reset score.
    score = DEFAULT_SCORE;
}

// Implement space junk here...

// Return score.
int get_score() {
    return score;
}