/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains everything related to the game timer 
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

// Global variables.
static double start_time = 0;
static double elapsed_time = 0;
static char timer[30];

// Global volatile variables.
static volatile int overflow_counter = 0;

// Get current time function which returns a real time counter based of timer 0.
double get_current_time() {
    double time = (overflow_counter * 256.0 + TCNT0) * 256.0/ 8000000.0;
    return time;
}

// Start timer function that intiates start_time with get_current_time().
void start_timer() {
    start_time = get_current_time();
}

// Calculate elapsed time function that return a double of the elapsed time since start_timer().
int calc_elapsed_time() {
    elapsed_time = get_current_time() - start_time;
    return elapsed_time;
}

// Format time function which formats a string to display the requirement for the elapsed time in the status
// display. Requires an int input which it will format into mm:ss.
void format_time(int time) {
    int minutes, seconds;

    minutes = time/ 60;
    seconds = time % 60;

    sprintf(timer, "Game Time: %02d:%02d", minutes, seconds);
}

// Get elapsed time function which returns a string of the elapsed time in mm:ss format.
char * get_elapsed_time() {
    if (start_time == 0) {
        format_time(start_time);
    } else {
        format_time(calc_elapsed_time());
    }
    return timer;
}

// Reset timers function.
void reset_timers() {
    start_time = 0;
    elapsed_time = 0;
    overflow_counter = 0;
}

// Return start_time.
double get_start_time() {
    return start_time;
}

// Interupt service routine to process timer overflow interrupts for timer 0.
ISR(TIMER0_OVF_vect) {
    // Increment counter_overflow.
    overflow_counter++;
}