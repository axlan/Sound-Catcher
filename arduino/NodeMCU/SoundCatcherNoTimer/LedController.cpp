#include "LedController.h"
#include <math.h>

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

int LedController::percent_to_ring(float percent) {
	return (int)max(0,
			        min(NUM_SPOKES - 1,
					round(percent * (float)(NUM_SPOKES)) - 1));
}

static CRGB* get_spoke_ptr(int spoke) {
	return leds + LedController::NUM_LED_PER_SPOKE * spoke;
}

static constexpr size_t get_spoke_size() {
	return sizeof(CRGB) * LedController::NUM_LED_PER_SPOKE;
}

static void mem_rev_cpy(CRGB* dst, CRGB* src, size_t size) {
	for (int i = 0; i < size; i++) {
		dst[size - 1 - i] = src[i];
	}
}

void LedController::rotate_spokes_cw() {
	CRGB tmp[NUM_LED_PER_SPOKE];
	memcpy(tmp, get_spoke_ptr(NUM_SPOKES - 1), get_spoke_size());
	for(int i = NUM_SPOKES-1; i > 0; i--) {
		mem_rev_cpy(get_spoke_ptr(i), get_spoke_ptr(i - 1), LedController::NUM_LED_PER_SPOKE);
	}
	mem_rev_cpy(get_spoke_ptr(0), tmp, LedController::NUM_LED_PER_SPOKE);
}

int LedController::percent_to_led(float percent) {
	return (int)max(0,
					min(NUM_LED_PER_SPOKE - 1,
					round(percent * (float)(NUM_LED_PER_SPOKE)) - 1));
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

void LedController::setspoke(const CRGB & c, int spoke, double percent) {
	setspoke(c, spoke, float(percent));
}

void LedController::setspoke(const CRGB & c, int spoke, float percent) {
	int leds = percent_to_led(percent);
	for(int i = 0; i <= leds; i++) {
		setled(c, spoke, i);
	}
}

void LedController::setspoke(const CRGB & c, int spoke, int leds) {
	for(int i = 0; i <= leds; i++) {
		setled(c, spoke, i);
	}
}

void LedController::setall(const CRGB & c) {
	for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
		leds[i] = c;
	}
}

void LedController::repaint()
{
	FastLED.show();
}
