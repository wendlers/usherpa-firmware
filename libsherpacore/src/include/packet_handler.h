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

#ifndef __PACKETHANDLER_H_
#define __PACKETHANDLER_H_

#include "packet.h"

/**
 * Start byte indicating IN-bound packets
 */
#define PACKET_INBOUND_START		0x24

/**
 * Start byte indicating OUT-bound packets
 */
#define PACKET_OUTBOUND_START		0x2B

/**
 * Start byte indicating OUT-bound packets
 */
#define PACKET_OUTBOUND_START_IR	0x21


/**
 * List of IN-bound packet receive handlers
 */
extern packet_rcv_handlers packet_handlers;

/**
 * Initialize packet handler
 */
void packet_handler_init();

/**
 * Do packet handling (by reading it from the underlaying input stream
 * and sending it to the packet handler assigned).
 */
void packet_do_handle();

#endif
