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
#include "space_junk.h"
#include "displays.h"

// Global variables.
int simulation_over = 0; // false.
int paused = 1; // true.

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
    // Setup deflector shield.
    setup_deflector_shield();
    // Setup starfighter.
    setup_starfighter();
    // Setup space junk.
    setup_space_junk();
}

// A reset function to reset the simulation.
void reset() {
    // Pause the game.
    paused = 1;
    set_display_paused(1);
    // Reset deflector shield.
    setup_deflector_shield();
    // Reset starfighter.
    setup_starfighter();
    // Reset space junk.
    setup_space_junk();
    // Draw all.
    draw_all();
    // Reset timers.
    reset_timers();
}

// A pause function which flips the paused variable.
void pause() {
    if (paused == 1) {
        paused = 0;
        set_display_paused(0);
    } else {
        paused = 1;
        set_display_paused(1);
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