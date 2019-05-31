/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains everything related to the starfighter and the deflector shield 
for the Asteroid Apocalypse teensypewpew program.

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

// Global Deflector Shield variables.
#define DEFLECTOR_SHIELD_ROW 39

// Draw Deflector Shield function.
void draw_deflector_shield() {
    for (int i = 0; i < LCD_X; i++) {
        // Draw a pixel at a every even x-coord.
        if (i % 2) {
            draw_pixel(i, DEFLECTOR_SHIELD_ROW, FG_COLOUR);
        } else {
            continue;
        }
    }
}

// Global Starfighter variables.
#define STARFIGHTER_WIDTH 14
#define STARFIGHTER_HEIGHT 7
static uint8_t starfighter_x, starfighter_y;
static int starfighter_velocity;
static int starfighter_direction;
static uint8_t starfighter_original[STARFIGHTER_WIDTH] = {
    0b11110000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b11100000,
    0b11100000,
    0b11111110,
    0b11111110,
    0b11100000,
    0b11100000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b11110000,
};

// Setup Starfighter in the middle of the bottom 7 rows of the LCD.
void setup_starfighter() {
    starfighter_x = (LCD_X/ 2) - (STARFIGHTER_WIDTH/ 2);
    starfighter_y = (LCD_Y - STARFIGHTER_HEIGHT);
    // Assign a random direction (either left or right) with velocity of 1.
    starfighter_velocity = 1;
    starfighter_direction = rand() % 2; // 0 = Left, 1 = Right.
}
// Draw the Starfighter with the center being (starfighter_x, starfighter_y).
void draw_starfighter() {
    LCD_CMD(lcd_set_function, lcd_instr_basic | lcd_addr_horizontal);
    LCD_CMD(lcd_set_x_addr, starfighter_x);
    LCD_CMD(lcd_set_y_addr, starfighter_y / 8);

    for (int i = 0; i < STARFIGHTER_WIDTH; i++) {
        LCD_DATA(starfighter_original[i]);
    }
}

// Update the Starfighter.
void update_starfighter() {
    // If velocity == 0, don't update starfighter.
    if (starfighter_velocity == 0) {
        return;
    } else {
        // If direction == 0 and if move doesn't overlap the left wall, move left.
        if (starfighter_direction == 0 && (starfighter_x - 1) > 0) {
            starfighter_x--;
        }
        // If direction == 1 and if move doesn't overlap the right wall, move right.
        else if (starfighter_direction == 1 && (starfighter_x + 1) <= (LCD_X - STARFIGHTER_WIDTH)) {
            starfighter_x++;
        } 
        // Else don't update starfighter.
        else {
            return;
        }
    }
}

// Change the Starfighter direction.
void change_starfighter_direction(int direction) {
    // If starfighter is currently moving in the same direction, do nothing.
    if (starfighter_velocity == 1 && starfighter_direction == direction) {
        return;
    }
    // Else if starfighter is currently moving in the opposite direction, stop it. 
    else if (starfighter_velocity == 1) {
        starfighter_velocity = 0;
    }
    // Else flip the current direction and make the starfighter move again. 
    else {
        starfighter_direction = direction;
        starfighter_velocity = 1;
    }
}

// Return starfighter_velocity.
uint8_t get_starfighter_velocity() {
    return starfighter_velocity;
}

// Return starfighter_direction.
uint8_t get_starfighter_direction() {
    return starfighter_direction;
}

// Set starfighter_velocity function.
void set_starfighter_velocity(uint8_t new_velocity) {
    starfighter_velocity = new_velocity;
}

// Set starfighter_direction function.
void set_starfighter_direction(uint8_t new_direction) {
    starfighter_direction = new_direction;
}