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

#define SCRATCH_DATA_REQUEST	0x01

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

/*
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
*/
}

interrupt(PORT1_VECTOR) PORT1_ISR(void) {

	/*
	if((P1IE & BIT0) == BIT0 && (P1IFG & BIT0) == BIT0 ) {
    	P1IFG &= ~BIT0;                 		
	}
	*/
}

void build_scratch_pkt(unsigned char * packet, int channel, int value)
{
	unsigned char upper_data= (unsigned char)((value&(unsigned int)0x380) >> 7);
	unsigned char lower_data= (unsigned char)(value&0x7f);

	*packet++ = ((1 << 7) | (channel << 3) | (upper_data));
	*packet++ = lower_data;
}

void send_scratch_pkt(unsigned char* packet)
{
	serial_send_blocking(*packet++);
	delay(400);
	serial_send_blocking(*packet++);
	delay(400);
}

int main(void)
{
	unsigned char c; 
	unsigned char data_packet[] = { 0, 0 }; 

	clock_init();

	pin_reserve(PIN_1_1);
	pin_reserve(PIN_1_2);

	serial_init(9600);
	cio_print("Start\n");

	setup();
	
	for(;;) {

		c = serial_recv_blocking();

		if(c == SCRATCH_DATA_REQUEST) {	
			
			// Send the ID packet
			build_scratch_pkt(data_packet, 15, 0x04);
			send_scratch_pkt(data_packet);
			
			// Read/Report channel 0 (Resistance-D)
			build_scratch_pkt(data_packet, 0, 1);
			send_scratch_pkt(data_packet);
			
			// Read/Report Channel 1 (Resistance-C)
			build_scratch_pkt(data_packet, 1, 2);
			send_scratch_pkt(data_packet);

			// Read/Report Channel 2 (Resistance-B)
			build_scratch_pkt(data_packet, 2, 3);
			send_scratch_pkt(data_packet);			

			// Read/Report Channel 3 (Button)
			build_scratch_pkt(data_packet, 3, 4);
			send_scratch_pkt(data_packet);	
			
			// Read/Report Channel 4 (Resistance-A)
			build_scratch_pkt(data_packet, 4, 5);
			send_scratch_pkt(data_packet);		

			// Read/Report Channel 5 (LIGHT)
			build_scratch_pkt(data_packet, 5, 6);
			send_scratch_pkt(data_packet);	

			// Read/Report Channel 6(Sound)
			build_scratch_pkt(data_packet, 6, 7);
			send_scratch_pkt(data_packet);			
			
			//Read/Report Channel 7(Slider)
			build_scratch_pkt(data_packet, 7, 8);
			send_scratch_pkt(data_packet);		
		}
	}

	return 0;
}


