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

int mutex_flag[2];
int mutex_turn;

void mutex_acquire(int lockId) {

	int other			= 1 - lockId;

	mutex_flag[lockId] 	= 1;
    mutex_turn 			= lockId;

    while (mutex_flag[other] == 1 && mutex_turn == lockId) {
		__asm__("nop");
    }
}

void mutex_release(int lockId) {

	mutex_flag[lockId] = 0;
}

