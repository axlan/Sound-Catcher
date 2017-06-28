#include "TestPatterns.h"
#include "LEDController.h"
#include "LogBuffer.h"

update_red_led_test::update_red_led_test():
	s_delay_ms(200),
	s_single_not_spoke(true),
	spoke_idx(0),
	led_idx(0){
		add_val("delay (ms)", &s_delay_ms);
		add_val("single not spoke", &s_single_not_spoke);
	}

void update_red_led_test::update() {
	if (s_single_not_spoke.get_bool()) {
		LedController::setled(CRGB::Black, spoke_idx, led_idx);
		led_idx++;
		if (led_idx == LedController::NUM_LED_PER_SPOKE) {
			led_idx = 0;
			spoke_idx = (spoke_idx+1) % LedController::NUM_SPOKES;
		}
		LedController::setled(CRGB::Red, spoke_idx, led_idx);
	} else {
		LedController::setspoke(CRGB::Black, spoke_idx);
		spoke_idx = (spoke_idx+1) % LedController::NUM_SPOKES;
		LedController::setspoke(CRGB::Red, spoke_idx);
	}
	LedController::repaint();
	delay(s_delay_ms.get_int());
}

void update_blank::update() {

  LogBuffer::Write("Test");
	LedController::setall(CRGB::Black);
	
	LedController::repaint();
  delay(100);
}