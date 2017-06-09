#pragma once

#include "FastLED.h"

class LedController
{
public:
	
    static const int NUM_SPOKES = 12;
    static const int NUM_LED_PER_SPOKE = 8; 
	
	static void init();
	
    static void rotate_spokes_cw();

    static int percent_to_ring(float percent);

    static int percent_to_led(float percent);

    static void setring(const CRGB& c, int ring);

    static void setring(const CRGB& c, int band, float percent);

    static void setled(const CRGB& c, int spoke, int led);

    static void setspoke(const CRGB& c, int spoke);

    static void setspoke(const CRGB& c, int spoke, float percent);

    static void setall(const CRGB& c);
	
	static void repaint();
	
};
