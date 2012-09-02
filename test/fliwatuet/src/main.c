/*
 * This file is part of the uSherpa project.
 *
 * Copyright (C) 2012 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <msp430.h>
#include <legacymsp430.h>

#include "serial.h"
#include "conio.h"
#include "pin.h"

#define OFF		0
#define ON 		1
#define TOGGLE	2

int mdc = 0;
int lame_cnt = 0;

#define MODE_IDLE		0
#define MODE_DOODLE		1
#define MODE_MOTOR		2
#define MODE_EYETOG1	3
#define MODE_EYETOG2	4

volatile int mode = MODE_IDLE;

void clock_init(void)
{
	WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
	__bis_SR_register(GIE);
}

void delay(int d) 
{
	volatile unsigned long i = d;

	do (i--);
	while (i != 0);
}

void setup() {

	cio_print("setting up pins\n\r");

	pin_setup(PIN_1_0, PIN_FUNCTION_INPUT_PULLDOWN); 
	pin_setup(PIN_1_4, PIN_FUNCTION_INPUT_PULLDOWN); 
	pin_setup(PIN_1_5, PIN_FUNCTION_INPUT_PULLDOWN); 
	pin_setup(PIN_1_7, PIN_FUNCTION_INPUT_PULLDOWN); 

	pin_exti_function(PIN_1_0, PIN_FUNCTION_EXTI_LOWHIGH, 0); 
	pin_exti_function(PIN_1_4, PIN_FUNCTION_EXTI_LOWHIGH, 0); 
	pin_exti_function(PIN_1_5, PIN_FUNCTION_EXTI_LOWHIGH, 0); 
	pin_exti_function(PIN_1_7, PIN_FUNCTION_EXTI_LOWHIGH, 0); 

	pin_setup(PIN_2_0, PIN_FUNCTION_OUTPUT);
	pin_setup(PIN_2_3, PIN_FUNCTION_OUTPUT);
	pin_setup(PIN_2_4, PIN_FUNCTION_OUTPUT);
	pin_setup(PIN_2_5, PIN_FUNCTION_OUTPUT);

	pin_setup(PIN_1_6, PIN_FUNCTION_PWM);
	pin_setup(PIN_2_2, PIN_FUNCTION_PWM);

	pin_pwm_function(PIN_1_6, 5000);
 	pin_pwm_control(PIN_1_6, mdc);

}

void tone(int h, int d) {

	pin_pwm_function(PIN_2_2, 3000 + h);
	pin_pwm_control(PIN_2_2, 40);

	delay(d);

	pin_pwm_control(PIN_2_2, 0);
}

void doodle(int offs) {

	cio_print("doodle a melody\n\r");

	int d; 

	for(d = 0; d < 1000; d+= 100) {
		tone(offs + d, 10000);
	}
}

void motor_speed() {

	mdc = mdc + 50;

	if(mdc >= 300) mdc = 0;

	cio_printf("setting moter speed to %i\n\r", mdc);

	pin_pwm_control(PIN_1_6, (unsigned char)mdc);
}

void set_pin(unsigned char pin, unsigned char mode) {

	if(mode == OFF) {
		pin_clear(pin);
	}
	else if(mode == ON) {
		pin_set(pin);
	}
	else {
		pin_toggle(pin);
	}	
}

void orange_led(unsigned char state) {
	set_pin(PIN_2_3, state);
}

void blue_led(unsigned char state) {
	set_pin(PIN_2_4, state);
}

void green_led(unsigned char state) {
	set_pin(PIN_2_5, state);
}

void red_led(unsigned char state) {
	set_pin(PIN_2_0, state);
}

interrupt(PORT1_VECTOR) PORT1_ISR(void) {

	if((P1IE & BIT0) == BIT0 && (P1IFG & BIT0) == BIT0 ) {
    	P1IFG &= ~BIT0;                 		
		if(mode == MODE_IDLE) {
			mode = MODE_DOODLE;
		}
	}
	if((P1IE & BIT4) == BIT4 && (P1IFG & BIT4) == BIT4 ) {
    	P1IFG &= ~BIT4;                 		
		if(mode == MODE_IDLE) {
			mode = MODE_MOTOR;
		}
	}
	if((P1IE & BIT5) == BIT5 && (P1IFG & BIT5) == BIT5 ) {
    	P1IFG &= ~BIT5;                 		
		if(mode == MODE_IDLE) {
			mode = MODE_EYETOG1;
		}
	}
	if((P1IE & BIT7) == BIT7 && (P1IFG & BIT7) == BIT7 ) {
    	P1IFG &= ~BIT7;                 		
		if(mode == MODE_IDLE) {
			mode = MODE_EYETOG2;
		}
	}
}

int main(void)
{

	unsigned int cnt = 0;

	clock_init();

	pin_reserve(PIN_1_1);
	pin_reserve(PIN_1_2);

	serial_init(9600);
	cio_print("** uSherpa - fliwatuet **\n\r");

	setup();
	
	red_led(ON);

	for(;;) {

		if(mdc != 0) {
			if(cnt % (5000 - (mdc * 15))  == 0) {
				blue_led(TOGGLE);
			}

			if(cnt % 5000 == 0) {
				red_led(TOGGLE);
				green_led(TOGGLE);
			}
		}

		if(lame_cnt >= 100) {
			cio_print("playing lame doodle\n\r");
			doodle(3000);
			lame_cnt = 0;
		}

		if(mode != MODE_IDLE) {
			cio_printf("mode is %i\n\r", mode);
			lame_cnt = 0;
			switch(mode) {
			
				case(MODE_DOODLE):
					orange_led(ON);
					doodle(1500);
					orange_led(OFF);
					break;

				case(MODE_MOTOR):
					tone(1500, 5000);
					motor_speed();
					break;

				case(MODE_EYETOG1):
					tone(1500, 5000);
					green_led(TOGGLE);
					break;

				case(MODE_EYETOG2):
					tone(1500, 5000);
					red_led(TOGGLE);
					break;

			}

			mode = MODE_IDLE;
		}		
		else {
			if(cnt % 5000 == 0) {
				lame_cnt++;
				cio_printf("setting lame_count to %i\n\r", lame_cnt);
			}
		}	

		cnt++;
	}

	return 0;
}
