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

void draw_deflector_shield();
void setup_starfighter();
void draw_starfighter();
void update_starfighter();
void change_starfighter_direction(int direction);
void animate_starfighter();
uint8_t get_starfighter_velocity();
uint8_t get_starfighter_direction();
void set_starfighter_velocity(uint8_t new_velocity);
void set_starfighter_direction(uint8_t new_direction);