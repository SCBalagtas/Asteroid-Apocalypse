/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains everything related to the displays (i.e. introduction display) 
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
#include "setup_teensy.h"
#include "starfighter.h"
#include "game_timer.h"
#include "space_junk.h"

/**
 * Introduction display.
 * A loop which displays the introduction display until the 
 * left button or 'r' is pressed.
 * */
void introduction() {
    int start_game = 0; // false.
    // While start_game == 0, keep displaying the introduction display.
    while (start_game == 0) {
        // Draw the introduction display.
        // Draw text and deflector shield with graphics.h.
        clear_screen();
        draw_string(22, 1, "n9998250", FG_COLOUR);
        draw_string(22, 15, "Asteroid", FG_COLOUR);
        draw_string(17, 25, "Apocalypse", FG_COLOUR);
        draw_deflector_shield();
        show_screen();
        // Draw and animate (update) the starfighter directly.
        draw_starfighter();
        animate_starfighter();
        // Recieve a character from serial communication with the computer and store it in 'ch'.
        int16_t ch = usb_serial_getchar();
        // Check if left button or 'r' is pressed.
        if (BIT_IS_SET(PINF, 6) || ch == 'r') start_game = 1; // End the introduction display loop.
    }
    // Seed a new seed for rand().
    srand(TCNT0);
    // Reset the starfighter before we leave this function.
    setup_starfighter();
}

/**
 * Quit display.
 * An infinite loop which displays student number on an inverted screen.
 * You cannot leave this loop.
 * */
void quit() {
    while(1) {
        // Draw the quit display.
        // Draw text with graphics.h.
        clear_screen();
        draw_string(22, 30, "n9998250", FG_COLOUR);
        show_screen();
        // Invert screen.
        LCD_CMD(lcd_set_display_mode, lcd_display_inverse);
    }
}

/**
 * Status display.
 * Draws over the teensy screen with the current status information iff game is paused.
 * Sends status information to the computer 
 **/
// Status display related global variables.
static int display_paused = 1;
static char lives_status[30];
static char score_status[30];
static char comp_status[100];
static char game_started[20] = "\r\nGame Started!\r\n";

// Send the status display to the computer and if paused, also display on the teensy.
void status_display() {
    // Format strings.
    sprintf(lives_status, "Lives: %d", get_lives());
    sprintf(score_status, "Score: %d", get_score());
    sprintf(comp_status, "\r\n%s\r\n%s\r\n%s\r\n", get_elapsed_time(), lives_status, score_status);
    // If paused draw status display on the teensy. 
    if (display_paused == 1) {
        // Draw the status display on the teensy.
        // Draw text with graphics.h.
        clear_screen();
        draw_string(1, 1, get_elapsed_time(), FG_COLOUR);
        draw_string(1, 11, lives_status, FG_COLOUR);
        draw_string(1, 21, score_status, FG_COLOUR);
        show_screen();
    }
    // Send comp_status to the computer.
    usb_serial_send(comp_status);
}

// Send a game started message as well as the display status.
void send_game_started() {
    usb_serial_send(game_started);
    status_display();
}

// Set diplay paused function.
void set_display_paused(int paused) {
    display_paused = paused;
}