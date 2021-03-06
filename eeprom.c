/*
EEPROM operations
    Copyright (C) 2018 pcm720 <pcm720@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see 
<http://www.gnu.org/licenses/>.
*/

#include "eeprom.h"
#include <avr/eeprom.h>
#include <util/atomic.h>

static inline uint8_t calculate_parity(uint8_t* state);

uint8_t eeprom_read(uint8_t* state) {
    uint8_t parity_bit;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        eeprom_read_block((void*)state, (void*)0, sizeof(*state));
        parity_bit = ((*state >> 0x7) & 1); // get parity bit read from EEPROM
        *state &= ~0x80; // clear parity bit from state
    }
    if (calculate_parity(state) != parity_bit) return 1; // read failed
    else return 0;
}

uint8_t eeprom_write(uint8_t state) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        state = state + (calculate_parity(&state) << 0x7); // 8th bit of state is a parity bit
        eeprom_update_block((const void*)&state, (void*)0, sizeof(state));
    }
    return eeprom_read(&state); // if write failed, returns 1
}

static inline uint8_t calculate_parity(uint8_t* state) {
    return ((*state >> 1) ^ (*state >> 2) ^ (*state >> 3)) & 1;
}