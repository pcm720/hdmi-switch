/*
Main definitions file for HDMI Switcher
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

#ifndef COMMON_H_
#define COMMON_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* switch states (used for outputState/selectedState)
    outputState/selectedState map:
        bits: 76543210
        76543: not used
        2:     output          (enabled — 1, disabled — 0)
        1:     selected switch (SW1 — 1, SW2 — 0)
        0:     selected port   (A — 0, B — 1)
*/
#define OFF   0x0
#define HDMI1 0x6 // 0b110
#define HDMI2 0x7 // 0b111
#define HDMI3 0x4 // 0b100
#define HDMI4 0x5 // 0b101

// inputs (PORT D)
#define B_OFF   0x1B
#define B_HDMI1 0x1E
#define B_HDMI2 0x1D
#define B_HDMI3 0x17
#define B_HDMI4 0x0F

// LEDs (PORT B)
#define LED_HDMI1 0x0
#define LED_HDMI2 0x1
#define LED_HDMI3 0x6
#define LED_HDMI4 0x7

// switch control pins configuration (PORT C)
#define SW1_SW2_OE  0x28 // 0b101000
#define SW3_OE      0x2  // 0b10
#define Dx_SEL_MASK 0x15 // 0b010101
#define PC_HDMI1    0x23 // 0b100011
#define PC_HDMI2    0x33 // 0b110011
#define PC_HDMI3    0xA  // 0b001010
#define PC_HDMI4    0xE  // 0b001110

/*
PC0 = SW3_Dx_SEL
PC1 = SW3_OE
PC2 = SW2_Dx_SEL
PC3 = SW2_OE
PC4 = SW1_Dx_SEL
PC5 = SW1_OE

            OFF                 HDMI1                  HDMI2                   HDMI3                   HDMI4
        OE      DxSEL        OE      DxSEL          OE      DxSEL           OE      DxSEL           OE      DxSEL
SW1     LOW     —            HIGH    LOW            HIGH    HIGH            LOW     —               LOW     —
SW2     LOW     —            LOW     —              LOW     —               HIGH    LOW             HIGH    HIGH
SW3     LOW     —            HIGH    HIGH           HIGH    HIGH            HIGH    LOW             HIGH    LOW
*/

#endif