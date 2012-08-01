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

#include "sherpacore.h"

int handle_packet_null(unsigned char length, unsigned char *data)
{
	send_status_packet(PACKET_RETURN_ACK);

	return PACKET_STAT_OK;
}

int handle_packet_system_info(unsigned char length, unsigned char *data)
{
	packet_data_out_system_info *pd = (packet_data_out_system_info *)&outp.data[0];

	outp.start	= PACKET_OUTBOUND_START;
	outp.length	= 7;
	outp.type 	= PACKET_OUT_SYSTEM_INFO;
	
	// TODO replace 0xCAFFEE with data from constants
	pd->board_type	= 0xCA;
	pd->mcu_type	= 0xFF;
	pd->firmware_rev= 0xEE;

	outp.crc	= packet_calc_crc(&outp);

	packet_send(&outp);

	return PACKET_STAT_OK;
}

int handle_packet_device_control(unsigned char length, unsigned char *data)
{
	// TODO define + implement
	send_status_packet(PACKET_RETURN_UNKNOWN);

	return PACKET_STAT_OK;
}

int handle_packet_pin_function(unsigned char length, unsigned char *data)
{
	int s;

	// check if length matches for packet-data
	if(length != 2) {
		send_status_packet(PACKET_RETURN_INAVLID_DATA);
		return PACKET_STAT_ERR_DATA;
	}

	packet_data_in_pin_function *pd = (packet_data_in_pin_function *)&data[0];

	if((s = pin_setup(pd->pin, pd->function)) != PACKET_STAT_OK) {
		send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
	}
	else {
		send_status_packet(PACKET_RETURN_ACK);
	}

	return s;
}

int handle_packet_pin_control(unsigned char length, unsigned char *data)
{
	int s = PACKET_STAT_OK;

	// check if length matches for packet-data
	if(length != 2) {
		send_status_packet(PACKET_RETURN_INAVLID_DATA);
		return PACKET_STAT_ERR_DATA;
	}

	packet_data_in_pin_control *pd = (packet_data_in_pin_control *)&data[0];

	switch(pd->control) {
		case PIN_CONTROL_CLEAR:
			if((s = pin_clear(pd->pin)) != PACKET_STAT_OK) {
				send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
			}
			else {
				send_status_packet(PACKET_RETURN_ACK);
			}
			break;
		case PIN_CONTROL_SET:
			if((s = pin_set(pd->pin)) != PACKET_STAT_OK) {
				send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
			}
			else {
				send_status_packet(PACKET_RETURN_ACK);
			}
			break;
		case PIN_CONTROL_TOGGLE:
			if((s = pin_toggle(pd->pin)) != PACKET_STAT_OK) {
				send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
			}
			else {
				send_status_packet(PACKET_RETURN_ACK);
			}
			break;
		case PIN_CONTROL_DIGITAL_READ:
			if((s = pin_digital_read(pd->pin)) < 0) {
				send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
			}
			else {
				packet_data_out_digital_pin_read *pdo = (packet_data_out_digital_pin_read *)&outp.data[0];

				outp.start	= PACKET_OUTBOUND_START;
				outp.length	= 6;
				outp.type 	= PACKET_OUT_DIGITAL_PIN_READ;
	
				pdo->pin   = pd->pin;
				pdo->state = s;

				outp.crc = packet_calc_crc(&outp);

				packet_send(&outp);
			}
			break;
		case PIN_CONTROL_ANALOG_READ:
			if((s = pin_analog_read(pd->pin)) < 0) {
				send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
			}
			else {
				packet_data_out_analog_pin_read *pdo = (packet_data_out_analog_pin_read *)&outp.data[0];

				outp.start	= PACKET_OUTBOUND_START;
				outp.length	= 7;
				outp.type 	= PACKET_OUT_ANALOG_PIN_READ;
	
				pdo->pin       = pd->pin;
				pdo->value_lsb = (0x00FF & s);
				pdo->value_msb = (0x0F00 & s) >> 8;

				outp.crc = packet_calc_crc(&outp);

				packet_send(&outp);
			}
			break;
		case PIN_CONTROL_PULSELENGTH_READ:
		case PIN_CONTROL_PULSELENGTH_READ_DHF:

			if(pd->control == PIN_CONTROL_PULSELENGTH_READ) {
				s = pin_pulselength_read(pd->pin);
			}
			else {
				s = pin_pulselength_read_dhf(pd->pin);
			}

			if(s < 0) {
				send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
			}
			else {
				packet_data_out_pulselength_read *pdo = 
							( packet_data_out_pulselength_read *)&outp.data[0];

				outp.start	= PACKET_OUTBOUND_START;
				outp.length	= 7;
				outp.type 	= PACKET_OUT_PULSELENGHT_READ;
	
				pdo->pin   = pd->pin;
				pdo->value_lsb = (0x00FF & s);
				pdo->value_msb = (0x7F00 & s) >> 8;

				outp.crc = packet_calc_crc(&outp);

				packet_send(&outp);
			}
			break;
		default:
			send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
			s = PACKET_STAT_ERR_DATA;
	}

	return s;
}

int handle_packet_pwm_function(unsigned char length, unsigned char *data)
{
	int s;

	// check if length matches for packet-data
	if(length != 3) {
		send_status_packet(PACKET_RETURN_INAVLID_DATA);
		return PACKET_STAT_ERR_DATA;
	}

	packet_data_in_pwm_function *pd = (packet_data_in_pwm_function *)&data[0];

	int p = (0x00FF & pd->period_lsb) | (0xFF00 & (pd->period_msb << 8));

	if((s = pin_pwm_function(pd->pin, p)) != PACKET_STAT_OK) {
		send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
	}
	else {
		send_status_packet(PACKET_RETURN_ACK);
	}

	return s;
}

int handle_packet_pwm_control(unsigned char length, unsigned char *data)
{
	int s;

	// check if length matches for packet-data
	if(length != 2) {
		send_status_packet(PACKET_RETURN_INAVLID_DATA);
		return PACKET_STAT_ERR_DATA;
	}

	packet_data_in_pwm_control *pd = (packet_data_in_pwm_control *)&data[0];

	if((s = pin_pwm_control(pd->pin, pd->duty_cycle)) != PACKET_STAT_OK) {
		send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
	}
	else {
		send_status_packet(PACKET_RETURN_ACK);
	}

	return s;
}

int handle_packet_external_interrupt_function(unsigned char length, unsigned char *data)
{
	int s;

	// check if length matches for packet-data
	if(length != 2) {
		send_status_packet(PACKET_RETURN_INAVLID_DATA);
		return PACKET_STAT_ERR_DATA;
	}

	packet_data_in_external_interrupt_function *pd = (packet_data_in_external_interrupt_function *)&data[0];

	if((s = pin_exti_function(pd->pin, pd->function)) != PACKET_STAT_OK) {
		send_status_packet(PACKET_RETURN_INVALID_PIN_COMMAND);
	}
	else {
		send_status_packet(PACKET_RETURN_ACK);
	}

	return PACKET_STAT_OK;
}

