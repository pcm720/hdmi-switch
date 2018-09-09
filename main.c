/*
HDMI Switcher code for ATmega88PA
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

#include <avr/sleep.h>
#include "common.h"
#include "switch.h"
#include "eeprom.h"

volatile uint8_t selectedInput;
volatile uint8_t timer = 11;

void init(void);
void mcu_sleep(void);
void timer_restart(void);

int main(void) {

    uint8_t outputState = 0x0;
    uint8_t coldStart = 1;

    if (eeprom_read(&outputState)) {     // read config from EEPROM
        outputState = 0x0;                 // reading failed, revert to default parameters
        eeprom_write(outputState);         // try to save defaults
    }

    init();

    while(1) {
        while (timer != 0) {
            if (selectedInput != outputState) {
                PORTB &= ~0xC3; // disable LEDs
                outputState = selectedInput;
                switch(outputState) {
                    case OFF:
                        switch_input(OFF, 0);
                        coldStart = 1;
                        break;
                    case HDMI1:
                        switch_input(PC_HDMI1, coldStart);
                        PORTB |= (1 << LED_HDMI1);
                        coldStart = 0;
                        break;
                    case HDMI2:
                        switch_input(PC_HDMI2, coldStart);
                        PORTB |= (1 << LED_HDMI2);
                        coldStart = 0;
                        break;
                    case HDMI3:
                        switch_input(PC_HDMI3, coldStart);
                        PORTB |= (1 << LED_HDMI3);
                        coldStart = 0;
                        break;
                    case HDMI4:
                        switch_input(PC_HDMI4, coldStart);
                        PORTB |= (1 << LED_HDMI4);
                        coldStart = 0;
                        break;
                }
            }
        }
        eeprom_write(outputState);
        mcu_sleep();
    }
}

void init(void) {
    DDRB = 0xC3; // enable LEDs
    DDRC = 0x3F; // initialize switch control pins as outputs
    PORTC = 0x0;
    DDRD = 0x0;  // set PD to input
    PORTD = 0x0; // disable pull-ups
    PRR = 0xEB;  // power reduction

    PCMSK2 = 0x3F;          // prepare interrupts for every input pin
    PCICR = 0x4;            // enable PCINT for PORTD
    TCCR1B |= (1 << WGM12); // setting up Timer/Counter1 to cause Compare Match A interrupt every second
    TIMSK1 |= (1 << OCIE1A);
    sei();
    OCR1A = 7812;
    TCCR1B |= ((1 << CS10) | (1 << CS12));
}

void mcu_sleep(void) {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
}

void timer_restart(void) {
    timer = 11;
    TCCR1B |= ((1 << CS10) | (1 << CS12));
}

/*-------------------------------------------------
                Interrupt vectors
-------------------------------------------------*/

ISR(PCINT2_vect) {
    uint8_t inputs = PIND;
    switch(inputs & 0x1F) {
        case (B_OFF):
            selectedInput = OFF;
            timer_restart();
            break;
        case (B_HDMI1):
            selectedInput = HDMI1;
            timer_restart();
            break;
        case (B_HDMI2): 
            selectedInput = HDMI2;
            timer_restart();
            break; 
        case (B_HDMI3):
            selectedInput = HDMI3;
            timer_restart();
            break;
        case (B_HDMI4):
            selectedInput = HDMI4;
            timer_restart();
            break;
    }
    _delay_ms(10);
}

ISR(TIMER1_COMPA_vect) {
    if (timer == 1) TCCR1B ^= ((1 << CS10) | (1 << CS12)); // stop the timer
    timer--;
}