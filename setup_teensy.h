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

void setup_teensy();