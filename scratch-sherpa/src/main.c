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

/**
 * This firmware implements the protocol used by the Scratch Sensor Board,
 * also known as Pico Board. 
 *
 * Note: Scratch is hardcoded to 38400 Bauds. Thus, it is not possible to 
 * use the build in serial to usb converter of the Launchpad which is 
 * limited to 9600 Bauds. 
 *
 * Note: The original Scratch Sensor Board has seven ADC channels and one
 * digital input. Since the 8 ADC channels of the Launchpad interferre with
 * the RX/TX pins of the hard-uart, it is not possible to suport all of the
 * 7 channels.
 */

#include <msp430.h>
// #include <legacymsp430.h>

#include "serial.h"
// #include "conio.h"
#include "pin.h"

#define SCRATCH_DATA_REQUEST	0x01

#define BAUDRATE				38400
#define COMM_BLINK_RATE			25

#define COMM					PIN_1_0
#define READY					PIN_1_6
#define BUTTON					PIN_1_3
#define RESA					PIN_1_4
#define RESB					PIN_1_5
#define RESC					PIN_1_7

/**
 * Defines a single channel of the Scratch Sensor Board
 */
typedef struct {

	/**
 	 * Channel ID (0..7)
 	 */
	unsigned char id;

	/**
	 * Value sampled for this channel
	 */
	int value;

} ssb_channel;

/**
 * All the channels of the Scratch Sensor Board
 */
typedef struct {
	
	/**
 	 * The slider resistor
 	 */
	ssb_channel slider;

	/**
	 * The light sensor
	 */
	ssb_channel light;

	/**
     * The sound sensor
     */
	ssb_channel sound;

	/**
	 * The push button
	 */
	ssb_channel button;

	/**
	 * Resistor A
	 */
	ssb_channel resa;

	/**
 	 * Resistor B
 	 */
	ssb_channel resb;

	/**
 	 * Resistor C
 	 */
	ssb_channel resc;

	/**
 	 * Resistor D
 	 */
	ssb_channel resd;
	
} ssb_sample_data;

/**
 * Instance holding sample data, predefines channel IDs
 */
ssb_sample_data samples = {

	.slider = { 0x07, 0x00 },
	.light  = { 0x05, 0x00 },
	.sound  = { 0x06, 0x00 },
	.button = { 0x03, 0x00 },
	.resa   = { 0x04, 0x00 },
	.resb   = { 0x02, 0x00 },
	.resc   = { 0x01, 0x00 },
	.resd   = { 0x00, 0x00 },
};

/**
 * Initialize MCU clock etc.
 */
void clock_init(void)
{
	WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL  = CALDCO_1MHZ;
	__bis_SR_register(GIE);
}

#if 0
void delay(int d) 
{
	volatile unsigned long i = d;

	do (i--);
	while (i != 0);
}
#endif

/**
 * Setup the ports.
 */
void setup() 
{
	// red led
	pin_setup(COMM, PIN_FUNCTION_OUTPUT);

	// build in button
	pin_setup(BUTTON, PIN_FUNCTION_INPUT_PULLUP);

	// setup as analog in
	pin_setup(RESA  , PIN_FUNCTION_ANALOG_IN);
	
	// setup as analog in
	pin_setup(RESB  , PIN_FUNCTION_ANALOG_IN);

	// setup as analog in
	pin_setup(RESC  , PIN_FUNCTION_ANALOG_IN);

	// green led
	pin_setup(READY, PIN_FUNCTION_OUTPUT);

	// show that mcu is ready
	pin_set(READY);
}

/**
 * Read the sampleas, write them to {link}samples.
 */
void read_samples() 
{

	samples.slider.value = 0;

	samples.light.value  = 0;

	samples.sound.value  = 0;  

	samples.button.value = (pin_digital_read(BUTTON) ? 0xFFFF : 0);
 
	samples.resa.value   = pin_analog_read(RESA); 

	samples.resb.value   = pin_analog_read(RESB);  

	samples.resc.value   = pin_analog_read(RESC);  

	samples.resd.value   = 0; 
}

#if 0
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
#endif

/**
 * Transmit a tupel consiting of a channel ID and a sample value
 * over the serial line to the client.
 *
 * param [in] channel	channel ID (0..7)
 * param [in] value		sample value
 */
void xmit_tuple(int channel, int value) 
{
	unsigned char hi = (unsigned char)((value & (unsigned int)0x380) >> 7);
	unsigned char lo = (unsigned char)(value & 0x7f);

	serial_send_blocking((1 << 7) | (channel << 3) | (hi));
	serial_send_blocking(lo);
}

/**
 * Send all the samples to the client. Use data stored in {link}samples.
 */
void xmit_samples() 
{
	// send id
	xmit_tuple(0x0F, 0x04);
	
	// send samples
	xmit_tuple(samples.slider.id, samples.slider.value);
	xmit_tuple(samples.light.id , samples.light.value);
	xmit_tuple(samples.sound.id , samples.sound.value);
	xmit_tuple(samples.button.id, samples.button.value);
	xmit_tuple(samples.resa.id  , samples.resa.value);
	xmit_tuple(samples.resb.id  , samples.resb.value);
	xmit_tuple(samples.resc.id  , samples.resc.value);
	xmit_tuple(samples.resd.id  , samples.resd.value);
}

/**
 * The main routine.
 */
int main(void)
{
	unsigned char cnt = 0;
	unsigned char c; 

	clock_init();

	pin_reserve(PIN_1_1);
	pin_reserve(PIN_1_2);

	serial_init(BAUDRATE);

	setup();
	
	for(;;) {

		c = serial_recv_blocking();

		if(c == SCRATCH_DATA_REQUEST) {	
			
			if(cnt++ == COMM_BLINK_RATE) {
				// blink COMM to show data was requested
				pin_toggle(COMM);
				cnt = 0;
			}

			read_samples();
			xmit_samples();
		}
	}

	return 0;
}


#if 0
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

#endif
