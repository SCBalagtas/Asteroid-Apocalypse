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

// Global variables.
int simulation_over = 0; // false.
int paused = 1; // true.

// Draw all function.
void draw_all() {
    // Draw everything with graphics.h.
	clear_screen();
    draw_deflector_shield();
	show_screen();
    // Draw Starfighter directly.
    draw_starfighter();
}

// Setup the teensy and all objects in the simulation.
void setup(void) {
    // Setup teensy.
    setup_teensy();
    // Setup Starfighter.
    setup_starfighter();
    // Draw all.
    draw_all();
    // TEMPORARY!!!
    if (paused == 1) SET_BIT(PORTB, 3);
}

// A reset function to reset the simulation.
void reset() {
    setup();
}

// A pause function which flips the paused variable.
void pause() {
    if (paused == 1) {
        paused = 0;
        CLEAR_BIT(PORTB, 3);
        SET_BIT(PORTB, 2);        
        
    } else {
        paused = 1;
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
    // If joystick center is set, pause().
    if (BIT_IS_SET(PINB, 0)) {
        pause();
    }
}

// Interupt service routine to process timer overflow interrupts for timer 0.
ISR(TIMER0_OVF_vect) {
    // Implement anything that needs timer overflow here...
}

// This is the main process, runs the simulation.
void process(void) {
    do_operations();
    if (paused == 0) {
        // Run the game.
        update_starfighter();
    }
    draw_all();
}

// Run the program.
int main(void) {
	setup();

	for ( ;; ) {
		process();
		_delay_ms(10);
	}
}