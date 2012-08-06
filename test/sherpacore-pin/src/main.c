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

#include "serial.h"
#include "conio.h"
#include "pin.h"

void clock_init(void)
{
	WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
}

void dump_regs(const char *msg) 
{

	cio_printf("REGDUMP(%s): P1DIR=%x; P2DIR=%x; P1OUT=%x; P2OUT=%x; P1REN=%x; P2REN=%x; P1IN=%x, P2IN=%x\n\r",
				msg,
				P1DIR, P2DIR, P1OUT, P2OUT, P1REN, P2REN, P1IN, P2IN);
}

void delay() 
{
	volatile unsigned long i = 10000;

	do (i--);
	while (i != 0);
}

unsigned char adc2dc(int adc) 
{
	// adjust PWM duty cycle based on ADC reading
	// max. ADC value is 1024. Servo needs to be btw. 2.5 and 12.5% duty cycle
	// thus, 1024 = 10%, 1024 / 10 = 1%, delta % = adcin1 / (1024 / 10) 
	// duty cycle 255 = 100%, 2.5 = 1%
	// thus target duty cycle = 2.5% + delta % * 2.5
	return (2.5 * 2.5) + (unsigned char)((adc / (1024 / 10)) * 2.5);
}

int main(void)
{
	clock_init();

	pin_reserve(PIN_1_1);
	pin_reserve(PIN_1_2);

	serial_init(9600);

	cio_print("** uSherpa - librocketcore PIN test **\n\r");

	dump_regs("initial");

#if 0
	pin_setup(PIN_2_0, PIN_FUNCTION_INPUT_FLOAT);
	pin_setup(PIN_2_1, PIN_FUNCTION_OUTPUT);

	int pl = 0;

	cio_print("reading range finder...");
//	pl = pin_pulselength_read_dhf2p(PIN_2_0, PIN_2_1);
	pl = pin_pulselength_read_dhf(PIN_2_0);
	cio_printf(" -> pl=%i\n\r", pl);
#endif

	// invalid port
	if(pin_setup(0x30, PIN_FUNCTION_OUTPUT) == PIN_STAT_ERR_INVALPORT) {
		cio_print("0x30 is an invalid port\n\r");
	}
	
	// invalid pin
	if(pin_setup(0x2A, PIN_FUNCTION_OUTPUT) == PIN_STAT_ERR_INVALPIN) {
		cio_print("0x2A is an invalid pin\n\r");
	}

	// P1.1 + P1.2 are reserved for UART1
	if(pin_setup(PIN_1_1, PIN_FUNCTION_OUTPUT) == PIN_STAT_ERR_INVALPIN) {
		cio_print("0x11 is an invalid (reserved) pin\n\r");
	}
	if(pin_setup(PIN_1_2, PIN_FUNCTION_OUTPUT) == PIN_STAT_ERR_INVALPIN) {
		cio_print("0x12 is an invalid (reserved) pin\n\r");
	}

	// pins on port 2 do not support ADC
	int p;

	for(p = 0; p < 8; p++) { 
		if(pin_setup(PIN_2_0 + p, PIN_FUNCTION_ANALOG_IN) == PIN_STAT_ERR_UNSUPFUNC) {
			cio_printf("0x2%i does not support ADC\n\r", p);
		}
	}
	
	// set P1.0 + P1.6 + P2.5 to output (the build in LEDs)
	pin_setup(PIN_1_0, PIN_FUNCTION_OUTPUT);
	pin_setup(PIN_1_6, PIN_FUNCTION_OUTPUT);
	pin_setup(PIN_2_5, PIN_FUNCTION_OUTPUT);

	
	dump_regs("p1.0+p1.6+p2.5 output");

	// set P1.0 + P1.6 + P2.5 to HIGH
	pin_set(PIN_1_0);
	pin_set(PIN_1_6);
	pin_set(PIN_2_5);

	dump_regs("p1.0+p1.6+p2.5 set");

	// read P1.0 + P1.6 + p2.5 states
	cio_printf("P1.0 is %x\n\r", pin_digital_read(PIN_1_0));	
	cio_printf("P1.6 is %x\n\r", pin_digital_read(PIN_1_6));	
	cio_printf("P2.5 is %x\n\r", pin_digital_read(PIN_2_5));	
	
	// clear P1.0 + p1.6 + p2.5 to LOW 
	pin_clear(PIN_1_0);
	pin_clear(PIN_1_6);
	pin_clear(PIN_2_5);

	dump_regs("p1.0+p1.6+p2.5 clear");

	// read P1.0 + P1.6 + 2.5 states
	cio_printf("P1.0 is %x\n\r", pin_digital_read(PIN_1_0));	
	cio_printf("P1.6 is %x\n\r", pin_digital_read(PIN_1_6));	
	cio_printf("P2.5 is %x\n\r", pin_digital_read(PIN_2_5));	

	// toggle P1.0 + P1.6 + P2.5
	pin_toggle(PIN_1_0);
	pin_toggle(PIN_1_6);
	pin_toggle(PIN_2_5);

	dump_regs("p1.0+p1.6+p2.5 toggle");

	// read P1.0 + P1.6 states
	cio_printf("P1.0 is %x\n\r", pin_digital_read(PIN_1_0));	
	cio_printf("P1.6 is %x\n\r", pin_digital_read(PIN_1_6));	
	cio_printf("P2.5 is %x\n\r", pin_digital_read(PIN_2_5));	

	// toggle P1.0 + P1.6 + P2.5
	pin_toggle(PIN_1_0);
	pin_toggle(PIN_1_6);
	pin_toggle(PIN_2_5);

	dump_regs("p1.0+p1.6+p2.5 toggle");

	// read P1.0 + P1.6 states
	cio_printf("P1.0 is %x\n\r", pin_digital_read(PIN_1_0));	
	cio_printf("P1.6 is %x\n\r", pin_digital_read(PIN_1_6));	
	cio_printf("P2.5 is %x\n\r", pin_digital_read(PIN_2_5));	

	// set P1.3 to input float
	pin_setup(PIN_1_3, PIN_FUNCTION_INPUT_FLOAT);

	dump_regs("p1.3 input float");

	cio_print("Press button on P1.3 to continue ...");

	while(pin_digital_read(PIN_1_3)) __asm__("nop");

	cio_print(" OK\n\r");
	
	// set P2.3 to input pull-down 
	pin_setup(PIN_2_3, PIN_FUNCTION_INPUT_PULLDOWN);

	dump_regs("p2.3 input pull-down");

	cio_print("Press button on P2.3 to continue ...");

	while(!pin_digital_read(PIN_2_3)) __asm__("nop");

	cio_print(" OK\n\r");
	
	// set P2.4 to input pull-down 
	pin_setup(PIN_2_4, PIN_FUNCTION_INPUT_PULLUP);

	dump_regs("p2.4 input pull-up");

	cio_print("Press button on P2.4 to continue ...");

	while(pin_digital_read(PIN_2_4)) __asm__("nop");

	cio_print(" OK\n\r");

	int pl = 0;

	cio_print("Press button on P1.3 for pulselength read ...");

	delay(50000);

	pl = pin_pulselength_read(PIN_1_3);

	cio_printf(" OK, pl=%i\n\r", pl);

	cio_print("Press button on P2.3 for pulselength read ...");

	delay(50000);

	pl = pin_pulselength_read(PIN_2_3);

	cio_printf(" OK, pl=%i\n\r", pl);

	cio_print("Press button on P2.4 for pulselength read ...");

	delay(50000);

	pl = pin_pulselength_read(PIN_2_4);

	cio_printf(" OK, pl=%i\n\r", pl);
	
	pin_set(PIN_1_0);
	pin_clear(PIN_1_6);
	pin_clear(PIN_2_5);

	// set P1.5 to analog in
	int i = 0;

	cio_printf("setup 1.5 for analog in: %i\n\r", pin_setup(PIN_1_5, PIN_FUNCTION_ANALOG_IN));
	dump_regs("p1.5 analog in");
	
	int adcin1 = pin_analog_read(PIN_1_5); 
	int adcin2 = 0; 
	cio_printf("Analog read p1.5: %x\n\r", adcin1);

	// set P2.2 to PWM with period of 20ms and duty cycle of 7.5%
	cio_printf("setup 2.2 for PWM: %i\n\r", pin_setup(PIN_2_2, PIN_FUNCTION_PWM));
	dump_regs("p2.2 PWM");

	// only one of the two possible pins on port two are allowed to be set to PWM
	cio_printf("setup 2.1 for PWM: %i\n\r", pin_setup(PIN_2_1, PIN_FUNCTION_PWM));

	// period 
	pin_pwm_function(PIN_2_2, 20000);
	pin_pwm_control(PIN_2_2, adc2dc(adcin1));

	while (1) {
		delay();

		pin_toggle(PIN_1_0);
		pin_toggle(PIN_1_6);

		if(i++ % 2 == 0) {
			pin_toggle(PIN_2_5);
		}

		if(!pin_digital_read(PIN_1_3)) {
			pin_toggle(PIN_1_6);
			while(!pin_digital_read(PIN_1_3)) __asm__("nop");
		}

		adcin2 = pin_analog_read(PIN_1_5); 

		// only output ADC value if delta was more then 5
		if(adcin2 - adcin1 > 5 || adcin1 - adcin2 > 5) {
			adcin1 = adcin2;
			cio_printf("Analog read at p1.5: %x (%i)\n\r", adcin2, adcin2);
			pin_pwm_control(PIN_2_2, adc2dc(adcin1));
		}
	}

	return 0;
}
