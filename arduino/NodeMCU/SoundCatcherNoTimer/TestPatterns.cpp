#include "TestPatterns.h"
#include "LEDController.h"

update_red_1_led_test::update_red_1_led_test(int delay_ms):
	delay_ms(delay_ms),
	spoke_idx(0),
	led_idx(0){}

void update_red_1_led_test::update() {
	LedController::setled(CRGB::Black, spoke_idx, led_idx);
	led_idx++;
	if (led_idx == LedController::NUM_LED_PER_SPOKE) {
		led_idx = 0;
		spoke_idx = (spoke_idx+1) % LedController::NUM_SPOKES;
	}
	LedController::setled(CRGB::Red, spoke_idx, led_idx);
	LedController::repaint();
	delay(delay_ms);
}

update_red_8_led_test::update_red_8_led_test(int delay_ms):
  delay_ms(delay_ms),
  spoke_idx(0){}

void update_red_8_led_test::update() {
  LedController::setspoke(CRGB::Black, spoke_idx);
  spoke_idx = (spoke_idx+1) % LedController::NUM_SPOKES;
  LedController::setspoke(CRGB::Red, spoke_idx);
  LedController::repaint();
  delay(delay_ms);
}

void update_blank::update() {

	LedController::setall(CRGB::Black);
  delay(100);
}