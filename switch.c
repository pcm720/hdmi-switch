/*
Switching functions
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

#include "switch.h"
#include <util/atomic.h>

void switch_off(void) {
    PORTC &= ~SW1_SW2_OE;   // disable SW1_OE and SW2_OE
    _delay_ms(10);
    PORTC &= ~SW3_OE;       // disable SW3_OE
    _delay_ms(10);
    PORTC &= ~Dx_SEL_MASK; // clear Dx_SEL on all switches
}

void switch_input(uint8_t portConfig, uint8_t coldStart) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (coldStart == 0) switch_off();
        if (portConfig != OFF) {
            PORTC |= (portConfig & Dx_SEL_MASK);      // set Dx_SEL on all switches
            _delay_ms(500);
            PORTC |= (portConfig & SW1_SW2_OE);       // enable SWn_OE
            _delay_ms(10);
            PORTC |= (portConfig & SW3_OE);           // enable SW3_OE
        }
    }
}