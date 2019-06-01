/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the main function for the Asteroid Apocalypse teensypewpew program.

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
int simulation_over = 0; // false.
int paused = 1; // true.
int lives = 5;
int score = 0;

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
    while(simulation_over == 1) {
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
static char lives_status[30];
static char score_status[30];
static char comp_status[100];
static char game_started[20] = "\r\nGame Started!\r\n";

// Send the status display to the computer and if paused, also display on the teensy.
void status_display() {
    // Format strings.
    sprintf(lives_status, "Lives: %d", lives);
    sprintf(score_status, "Score: %d", score);
    sprintf(comp_status, "\r\n%s\r\n%s\r\n%s\r\n", get_elapsed_time(), lives_status, score_status);
    // If paused draw status display on the teensy. 
    if (paused == 1) {
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

// Draw all function.
void draw_all() {
    // Draw everything with graphics.h.
	clear_screen();
    draw_deflector_shield();
	show_screen();
    // Draw starfighter directly.
    draw_starfighter();
}

// Setup the teensy and all objects in the simulation.
void setup(void) {
    // Setup teensy.
    setup_teensy();
    // Setup starfighter.
    setup_starfighter();
    // TEMPORARY!!!
    if (paused == 1) SET_BIT(PORTB, 3);
}

// A reset function to reset the simulation.
void reset() {
    // Pause the game.
    paused = 1;
    // Reset starfighter.
    setup_starfighter();
    // Draw all.
    draw_all();
    // Reset timers.
    reset_timers();
    // TEMPORARY!!!
    CLEAR_BIT(PORTB, 2);
    SET_BIT(PORTB, 3);
}

// A pause function which flips the paused variable.
void pause() {
    if (paused == 1) {
        paused = 0;
        // TEMPORARY!!!
        CLEAR_BIT(PORTB, 3);
        SET_BIT(PORTB, 2);        
    } else {
        paused = 1;
        // TEMPORARY!!!
        CLEAR_BIT(PORTB, 2);
        SET_BIT(PORTB, 3);
    }
}

/**
 * Jump table which chooses the action (if any) which corresponds to a
 * input control being set
 * 
 * Parameters:
 *      none
 * */
void do_operations() {
    // Reviece a character from serial communication with the computer and store it in 'ch'.
    int16_t ch = usb_serial_getchar();
    // If left button is set or 'r' is recieved, reset().
    if (BIT_IS_SET(PINF, 6) || ch == 'r') {
        reset();
    }
    // Else if joystick center is set or 'p' is recieved, pause().
    else if (BIT_IS_SET(PINB, 0) || ch == 'p') {
        pause();
        // Start timer if start_time == 0 and send the game started message, 
        // when the user first unpauses the game.
        if (get_start_time() == 0) {
            start_timer();
            send_game_started();
        }
    }
    // Else if joystick left is set or 'a' is recieved, change starfighter direction to left.
    else if (BIT_IS_SET(PINB, 1) || ch == 'a') {
        change_starfighter_direction(0);
    }
    // Else if joystick right is set or 'd' is recieved, change starfighter direction to right.
    else if (BIT_IS_SET(PIND, 0) || ch == 'd') {
        change_starfighter_direction(1);
    }
    // Else if right button is set or 'q' is recieved, set simulation_over to 1.
    else if (BIT_IS_SET(PINF, 5) || ch == 'q') {
        simulation_over = 1;
    }
    // Else if joystick down is set or 's' is recieved, status_display().
    else if (BIT_IS_SET(PINB, 7) || ch == 's') {
        status_display();
    }
}

// This is the main process, runs the simulation.
void process(void) {
    if (simulation_over == 1) quit();
    do_operations();
    if (paused == 0) {
        // Run the game.
        update_starfighter();
        draw_all();
    }
}

// Run the program.
int main(void) {
	setup();
    introduction();
    draw_all();
	for ( ;; ) {
		process();
		_delay_ms(10);
	}
}