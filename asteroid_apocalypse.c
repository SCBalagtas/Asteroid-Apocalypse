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
}

// This is the main process, runs the simulation.
void process(void) {

}

// Run the program.
int main(void) {
	setup();

	for ( ;; ) {
		process();
		_delay_ms(10);
	}
}