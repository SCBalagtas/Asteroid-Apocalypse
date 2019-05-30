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
}
// Draw the Starfighter with the center being (starfighter_x, starfighter_y)
void draw_starfighter() {
    LCD_CMD(lcd_set_function, lcd_instr_basic | lcd_addr_horizontal);
    LCD_CMD(lcd_set_x_addr, starfighter_x);
    LCD_CMD(lcd_set_y_addr, starfighter_y / 8);

    for (int i = 0; i < STARFIGHTER_WIDTH; i++) {
        LCD_DATA(starfighter_original[i]);
    }
}