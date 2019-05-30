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
}

// Setup teensy.
void setup_teensy() {
    // Setup clock speed.
	set_clock_speed(CPU_8MHz);
    // Setup teensy inputs.
    setup_teensy_inputs();
    // Setup LCD.
	lcd_init(LCD_DEFAULT_CONTRAST);
}