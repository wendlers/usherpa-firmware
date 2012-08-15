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

#include "mutex.h"
#include "packet.h"

unsigned char packet_calc_crc(packet *pkt) {

	unsigned char i;
	unsigned char crc = 0;

	crc += pkt->start;
	crc += pkt->length;
	crc += pkt->type;

	for(i = 0; i < pkt->length - 4; i++) {
		crc += pkt->data[i];
	} 

	return crc;
}

int	packet_check_crc(packet *pkt) {

	if(packet_calc_crc(pkt) == pkt->crc) {
		return 1;
	}

	return 0;
}

int packet_send(packet *pkt) {

	unsigned char  i;

	// check length
	if(pkt->length - 4 > PACKET_MAX_DATA) {
		return PACKET_STAT_ERR_LENGTH;
	}
 
	// write start byte
	packet_byte_to_sendq(pkt->start);

	// write length
	packet_byte_to_sendq(pkt->length);

	// write type
	packet_byte_to_sendq(pkt->type);

	// write data
	for(i = 0; i < pkt->length - 4; i++) {	
		packet_byte_to_sendq(pkt->data[i]); 
	}

	// write crc 
	packet_byte_to_sendq(pkt->crc);
	
	return PACKET_STAT_OK;
}

int packet_send_excl(packet *pkt, int lockId) {

	int ret; 

	mutex_acquire(lockId);
	ret = packet_send(pkt);
	mutex_release(lockId);

	return ret;
}

int packet_receive(packet *pkt, unsigned char start) {

	unsigned char  i;

	// discard everything until start byte found in buffer
	while(packet_byte_from_rcvq() != start);

	pkt->start = start;

	// read length
	pkt->length = packet_byte_from_rcvq();
	
	// check length
	if(pkt->length - 4 > PACKET_MAX_DATA) {
		return PACKET_STAT_ERR_LENGTH;
	}
 
	// read type
	pkt->type = packet_byte_from_rcvq();
 	
	// read data
	for(i = 0; i < pkt->length - 4; i++) {	
		pkt->data[i] = packet_byte_from_rcvq();
	}
	
	// read crc 
	pkt->crc = packet_byte_from_rcvq();
	
	// check crc
	if(!packet_check_crc(pkt)) {
		return PACKET_STAT_ERR_CRC;
	}

	return PACKET_STAT_OK;
}

int packet_process_received(packet_rcv_handlers *rh, packet *pkt) {

	int i;

    for(i = 0; i < rh->count; i++) {
		if(rh->handlers[i].type == pkt->type) {
			// call handler function
    		return (rh->handlers[i].func)(pkt->length - 4, pkt->data);
		}
    }

	return PACKET_STAT_ERR_UNKPACK;
}
