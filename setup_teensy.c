/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains everything related to setting up the teensy
for the Asteroid Apocalypse teensypewpew program.

*/

// Import .h files.
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
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

// Setup teensy inputs (controls).
void setup_teensy_inputs() {
    // Configure input from joystick center.
    CLEAR_BIT(DDRB, 0);
    // Configure input from joystick left.
    CLEAR_BIT(DDRB, 1);
    // Configure input from joystick right.
    CLEAR_BIT(DDRD, 0);
    // Configure input from joystick up.
    CLEAR_BIT(DDRD, 1);
    // Configure input from joystick down.
    CLEAR_BIT(DDRB, 7);
    // Configure input from left button.
    CLEAR_BIT(DDRF, 6);
    // Configure input from right button.
    CLEAR_BIT(DDRF, 5);
    // Initialise ADC to configure input from potentiometers.
    adc_init();

    // TEMPORARY!!!
    SET_BIT(DDRB, 2);
    SET_BIT(DDRB, 3);
}

// Setup timer.
void setup_timer() {
    // Initialise Timer 0 in normal mode so that it overflows 
	// with a period of approximately 0.008 seconds.
    TCCR0A = 0;
    TCCR0B = 4;
    // Enable timer overflow interrupt for Timer 0.
    TIMSK0 = 1;
    // Turn on interrupts.
    sei();
}

// Setup usb serial connection.
void setup_usb_serial() {
	// Set up usb serial connection and display message.
	draw_string(10, 10, "Connect USB...", FG_COLOUR);
	show_screen();
	usb_init();
	while ( !usb_configured() ) {
		// Block until USB is ready.
	}
	clear_screen();
	draw_string(10, 10, "USB connected", FG_COLOUR);
	show_screen();
}

// Define a usb serial send function to send a string the computer.
void usb_serial_send(char * message) {
	// Cast to avoid "error: pointer targets in passing argument 1 
	//	of 'usb_serial_write' differ in signedness"
	usb_serial_write((uint8_t *) message, strlen(message));
}

// Setup teensy.
void setup_teensy() {
    // Setup clock speed.
	set_clock_speed(CPU_8MHz);
    // Setup teensy inputs.
    setup_teensy_inputs();
    // Setup timer 0 in normal mode so that it overflows every 0.008 seconds.
    setup_timer();
    // Setup LCD.
	lcd_init(LCD_DEFAULT_CONTRAST);
    // Setup usb serial connection
    setup_usb_serial();
}