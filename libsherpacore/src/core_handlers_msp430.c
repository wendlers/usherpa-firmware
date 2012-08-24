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

#include "sherpacore.h"

// definition and state of trigger counters
extern unsigned char pin_exti_trigger_count[16][2];

// keep track of external interrupts that changed a pin status from low to high
extern unsigned int exti_lowhigh_flags;

// keep track of external interrupts that changed a pin status from high to low
extern unsigned int exti_highlow_flags;

void process_exti() 
{
	packet outp;

	unsigned int bit;

	int i;

	if((exti_lowhigh_flags | exti_highlow_flags) == 0) {
		// nothing to do here
		return;
	}

	packet_data_out_digital_pin_read *pdo = (packet_data_out_digital_pin_read *)&outp.data[0];

	outp.start	= PACKET_OUTBOUND_START_IR;
	outp.length	= 6;
	outp.type 	= PACKET_OUT_DIGITAL_PIN_READ;

	for(i = 0; i < 16; i++) {

		bit = 0x01 << i;

		if((exti_lowhigh_flags & bit) == bit) {
			pdo->state = 1;
		}
		else if((exti_highlow_flags & bit) == bit) {
			pdo->state = 0;
		}
		else {
			continue;
		}

		if(i >= 8) {
			pdo->pin = PIN_2_0 + (i - 8);
		}
		else {
			pdo->pin = PIN_1_0 + i;
		}

		outp.crc = packet_calc_crc(&outp);
		packet_send(&outp);
	}
}

int handle_packet_reset(unsigned char length, unsigned char *data)
{
	send_status_packet(PACKET_RETURN_ACK);
	
	// give send packet some time before we reset ...
	volatile unsigned long i = 10000;

	do (i--);
	while (i != 0);
	
	// make watchdog bite ...
	WDTCTL = 0;

	return PACKET_STAT_OK;
}

interrupt(PORT1_VECTOR) PORT1_ISR(void)
{
//	packet outp;

	int idx;

	unsigned char i;
	unsigned char bit;

#if 0
	packet_data_out_digital_pin_read *pdo = (packet_data_out_digital_pin_read *)&outp.data[0];

	outp.start	= PACKET_OUTBOUND_START_IR;
	outp.length	= 6;
	outp.type 	= PACKET_OUT_DIGITAL_PIN_READ;
#endif
	
	for(i = 0; i < 8; i++) {
	
		bit = 0x01 << i;

		if((P1IE & bit) == bit && (P1IFG & bit) == bit) {

			P1IFG &= ~bit;			// reset IR flag

			idx	= i;				// index in trigger count array

			// send interrupt packet only if trigger count is reached
			if(++pin_exti_trigger_count[idx][1] >= pin_exti_trigger_count[idx][0]) {  

				if(pin_exti_trigger_count[idx][0] > 0) {
	 	    		P1IE  &= ~bit;		// disable interrupt
				}

				pin_exti_trigger_count[idx][1] = 0; 

				// set interrupt flag for this bit
				if((P1IES & bit) == bit) {
					exti_lowhigh_flags |=  bit;
					exti_highlow_flags &= ~bit;
				} 
				else {
					exti_lowhigh_flags &= ~bit;
					exti_highlow_flags |=  bit;
				}
				
#if 0
				pdo->pin   = PIN_1_0 + i;
				pdo->state = ((P1IES & bit) ? 0 : 1);

				outp.crc = packet_calc_crc(&outp);

				packet_send_excl(&outp, IRQ);
#endif
			}
		}
	}
}

interrupt(PORT2_VECTOR) PORT2_ISR(void)
{
//	packet outp;

	int idx;

	unsigned char i;
	unsigned char bit;

#if 0
	packet_data_out_digital_pin_read *pdo = (packet_data_out_digital_pin_read *)&outp.data[0];

	outp.start	= PACKET_OUTBOUND_START_IR;
	outp.length	= 6;
	outp.type 	= PACKET_OUT_DIGITAL_PIN_READ;
#endif
	
	for(i = 0; i < 8; i++) {
	
		bit = 0x01 << i;

		if((P2IE & bit) == bit && (P2IFG & bit) == bit) {

			P2IFG &= ~bit;			// reset IR flag

			idx	= 8 + i;			// index in trigger count array

			// send interrupt packet only if trigger count is reached
			if(++pin_exti_trigger_count[idx][1] >= pin_exti_trigger_count[idx][0]) {  

				if(pin_exti_trigger_count[idx][0] > 0) {
	 	    		P2IE  &= ~bit;		// disable interrupt
				}

				pin_exti_trigger_count[idx][1] = 0; 
				//
				// set interrupt flag for this bit
				if((P2IES & bit) == bit) {
					exti_lowhigh_flags |=  (bit << 8);
					exti_highlow_flags &= ~(bit << 8);
				} 
				else {
					exti_lowhigh_flags &= ~(bit << 8);
					exti_highlow_flags |=  (bit << 8);
				}
#if 0
				pdo->pin   = PIN_2_0 + i;
				pdo->state = ((P2IES & bit) ? 0 : 1);

				outp.crc = packet_calc_crc(&outp);

				packet_send_excl(&outp, IRQ);
#endif
			}
		}
	}
}
