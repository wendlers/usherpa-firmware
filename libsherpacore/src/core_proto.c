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

#include "core_proto.h"
#include "packet_handler.h"

/**
 * OUT-bound packet instance.
 */
packet outp;

/**
 * IN-bound packet instance.
 */
packet inp;

void send_status_packet(unsigned char stat)
{
	packet_data_out_status *pd = (packet_data_out_status *)&outp.data[0];

	outp.start	= PACKET_OUTBOUND_START;
	outp.length	= 5;
	outp.type 	= PACKET_OUT_STATUS;
	pd->status  = stat;
	outp.crc	= packet_calc_crc(&outp);

	packet_send(&outp);
}

