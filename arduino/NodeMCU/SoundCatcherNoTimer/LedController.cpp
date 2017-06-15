#include "LedController.h"

#define NUM_LEDS_PER_STRIP 96
static CRGB leds[NUM_LEDS_PER_STRIP];
 
void LedController::init()
{
  FastLED.addLeds<NEOPIXEL, 1>(leds, NUM_LEDS_PER_STRIP);
  setall(CRGB::Black);
  repaint();
}

void LedController::setled(const CRGB &c, int spoke, int led) {
	spoke = (18 - spoke) % 12;
	if (spoke & 1) {
		leds[spoke * NUM_LED_PER_SPOKE + led] = c;
	} else {
		leds[(spoke + 1) * NUM_LED_PER_SPOKE  - led - 1] = c;
	}
}

void LedController::setspoke(const CRGB & c, int spoke) {
	for (int j = 0; j < NUM_LED_PER_SPOKE; j++) {
		setled(c, spoke, j);
	}
}

/*
void LedController::rotate_spokes_cw() {
	const CRGB & tmp[] = spokes[MyPanel.NUM_SPOKES-1];
	for(int i = MyPanel.NUM_SPOKES-1; i > 0; i--) {
		spokes[i] = spokes[i-1];
	}
	spokes[0] = tmp;
}

int LedController::percent_to_ring(float percent) {
	return (int)Math.max(0,
			Math.min(NUM_SPOKES - 1,
					Math.round(percent * (float)(NUM_SPOKES)) - 1));
}

int LedController::percent_to_led(float percent) {
	return (int)Math.max(0,
						 Math.min(NUM_LED_PER_SPOKE - 1,
								  Math.round(percent * (float)(NUM_LED_PER_SPOKE)) - 1));
}

void LedController::setring(const CRGB & c, int ring) {
	for (int j = 0; j < NUM_SPOKES; j++) {
		setled(c, j, ring);
	}
}

void LedController::setring(const CRGB & c, int band, float percent) {
	int leds = percent_to_ring(percent);
	for(int i = 0; i <= leds; i++) {
		setled(c, i, band);
	}
}

void LedController::setspoke(const CRGB & c, int spoke, float percent) {
	int leds = percent_to_led(percent);
	for(int i = 0; i <= leds; i++) {
		setled(c, spoke, i);
	}
}
*/

void LedController::setall(const CRGB & c) {
	for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
		leds[i] = c;
	}
}

void LedController::repaint()
{
	FastLED.show();
}
