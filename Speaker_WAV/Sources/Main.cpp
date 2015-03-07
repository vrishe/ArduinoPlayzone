/*
 * Main.cpp
 *
 *  Created on: 31 џэт. 2015 у.
 *      Author: Admin
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include <stdint.h>


#include "sound/sound_gman_choose2.h"


#define PIN_SIGNAL_LED 13
#define PIN_SIGNAL_OUT 11 // 3 or 11 to use with Timer2
#define SIGNAL_AT(index) \
	(pgm_read_byte(SOUND_DATA + (index))/* - 0x80*/)


volatile uint16_t sample;
byte lastSample;


void stopPlayback()
{
    // Disable playback per-sample interrupt.
    TIMSK1 &= ~_BV(OCIE1A);

    // Disable the per-sample timer completely.
    TCCR1B &= ~_BV(CS10);

    // Disable the PWM timer.
    TCCR2B &= ~_BV(CS10);

    digitalWrite(PIN_SIGNAL_OUT, LOW);
}


#if PIN_SIGNAL_OUT == 11

// This is called at SAMPLE_RATE Hz to load the next sample.
ISR(TIMER1_COMPA_vect) {
    if (sample >= sizeof(SOUND_DATA)) {
        if (sample == sizeof(SOUND_DATA) + lastSample) {
            stopPlayback();
        } else {
			// Ramp down to zero to reduce the click at the end of playback.
			OCR2A = sizeof(SOUND_DATA) + lastSample - sample;
        }
    } else {
		OCR2A = SIGNAL_AT(sample);
    }
    ++sample;
}

#else // PIN_SIGNAL_OUT == 3

// This is called at SAMPLE_RATE Hz to load the next sample.
ISR(TIMER1_COMPA_vect) {
    if (sample >= sizeof(SOUND_DATA)) {
        if (sample == sizeof(SOUND_DATA) + lastSample) {
            stopPlayback();
        } else {
        	// Ramp down to zero to reduce the click at the end of playback.
        	OCR2B = sizeof(SOUND_DATA) + lastSample - sample;
        }
    } else {
    	OCR2B = SIGNAL_AT(sample);
    }
    ++sample;
}

#endif // PIN_SIGNAL_OUT == 11

void startPlayback()
{
    pinMode(PIN_SIGNAL_OUT, OUTPUT);

    // Set up Timer 2 to do pulse width modulation on the speaker
    // pin.

    // Use internal clock (datasheet p.160)
    ASSR &= ~(_BV(EXCLK) | _BV(AS2));

    // Set fast PWM mode  (p.157)
    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B &= ~_BV(WGM22);

#if PIN_SIGNAL_OUT == 11

        // Do non-inverting PWM on pin OC2A (p.155)
        // On the Arduino this is pin 11.
        TCCR2A = (TCCR2A | _BV(COM2A1)) & ~_BV(COM2A0);
        TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
        // No prescaler (p.158)
        TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

        // Set initial pulse width to the first sample.
        OCR2A = 0x7f; // SIGNAL_AT(0);

#else // PIN_SIGNAL_OUT == 3

        // Do non-inverting PWM on pin OC2B (p.155)
        // On the Arduino this is pin 3.
        TCCR2A = (TCCR2A | _BV(COM2B1)) & ~_BV(COM2B0);
        TCCR2A &= ~(_BV(COM2A1) | _BV(COM2A0));
        // No prescaler (p.158)
        TCCR2B = (TCCR2B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

        // Set initial pulse width to the first sample.
        OCR2B = 0x7f; // SIGNAL_AT(0);

#endif // PIN_SIGNAL_OUT == 11

    // Set up Timer 1 to send a sample every interrupt.
    cli();

    // Set CTC mode (Clear Timer on Compare Match) (p.133)
    // Have to set OCR1A *after*, otherwise it gets reset to 0!
    TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
    TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));

    // No prescaler (p.134)
    TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);

    // Set the compare register (OCR1A).
    // OCR1A is a 16-bit register, so we have to do this with
    // interrupts disabled to be safe.
    OCR1A = F_CPU / SAMPLE_RATE;    // 16e6 / 8000 = 2000

    // Enable interrupt when TCNT1 == OCR1A (p.136)
    TIMSK1 |= _BV(OCIE1A);

    lastSample = SIGNAL_AT(sizeof(SOUND_DATA)-1);
    sample = 0;
    sei();
}


void setup()
{
    pinMode(PIN_SIGNAL_LED, OUTPUT);
    digitalWrite(PIN_SIGNAL_LED, HIGH);

    pinMode(PIN_SIGNAL_OUT, OUTPUT);
    startPlayback();
}

void loop()
{
	// Spin loop during playback
	while(sample < sizeof(SOUND_DATA)) {
		/* Nothing to do */
	}
	std::terminate();
}

