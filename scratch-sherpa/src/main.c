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

#define COMM					PIN_1_0
#define READY					PIN_1_6
#define BUTTON					PIN_1_3
#define RESA					PIN_1_4

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

	// cio_print("setting up pins\n\r");

	// red led
	pin_setup(COMM, PIN_FUNCTION_OUTPUT);

	// build in button
	pin_setup(BUTTON, PIN_FUNCTION_INPUT_PULLUP);

	// setup as analog in
	pin_setup(RESA  , PIN_FUNCTION_ANALOG_IN);

	// green led
	pin_setup(READY, PIN_FUNCTION_OUTPUT);

	// show that mcu is ready
	pin_set(READY);
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
	unsigned char upper_data= (unsigned char)((value & (unsigned int)0x380) >> 7);
	unsigned char lower_data= (unsigned char)(value & 0x7f);

	*packet++ = ((1 << 7) | (channel << 3) | (upper_data));
	*packet++ = lower_data;
}

void send_scratch_pkt(unsigned char* packet)
{
	serial_send_blocking(*packet++);
	serial_send_blocking(*packet++);
}

int main(void)
{
	unsigned char cnt;
	unsigned char c; 
	unsigned char data_packet[] = { 0, 0 }; 

	clock_init();

	pin_reserve(PIN_1_1);
	pin_reserve(PIN_1_2);

	serial_init(38400);
	cio_print("Start\n");

	setup();
	
	cnt = 0;

	for(;;) {

		c = serial_recv_blocking();

		if(c == SCRATCH_DATA_REQUEST) {	
			
			if(cnt++ == 25) {
				// blink red led to show data was requested
				pin_toggle(COMM);
				cnt = 0;
			}

			// Send the ID packet
			build_scratch_pkt(data_packet, 15, 0x04);
			send_scratch_pkt(data_packet);
			
			// Read/Report channel 0 (Resistance-D)
			build_scratch_pkt(data_packet, 0, 0);
			send_scratch_pkt(data_packet);
			
			// Read/Report Channel 1 (Resistance-C)
			build_scratch_pkt(data_packet, 1, 0);
			send_scratch_pkt(data_packet);

			// Read/Report Channel 2 (Resistance-B)
			build_scratch_pkt(data_packet, 2, 0);
			send_scratch_pkt(data_packet);			

			// Read/Report Channel 3 (Button)
			if(!pin_digital_read(BUTTON)) {
				build_scratch_pkt(data_packet, 3, 0);
			}
			else {
				build_scratch_pkt(data_packet, 3, 0xFFFF);
			}
			send_scratch_pkt(data_packet);	
			
			// Read/Report Channel 4 (Resistance-A)
			build_scratch_pkt(data_packet, 4, pin_analog_read(RESA));
			send_scratch_pkt(data_packet);		

			// Read/Report Channel 5 (LIGHT)
			build_scratch_pkt(data_packet, 5, 0);
			send_scratch_pkt(data_packet);	

			// Read/Report Channel 6 (Sound)
			build_scratch_pkt(data_packet, 6, 0);
			send_scratch_pkt(data_packet);			
			
			//Read/Report Channel 7 (Slider)
			build_scratch_pkt(data_packet, 7, 0);
			send_scratch_pkt(data_packet);		
		}
	}

	return 0;
}


