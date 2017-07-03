#pragma once

#include "Patterns.h"

class update_led_test : public PatternBase{
private:
	int spoke_idx;
  int led_idx;
  Value s_single_not_spoke;
	Value s_delay_ms;
	Value s_hue;
	Value s_brightness;
public:
	update_led_test();
	void update();
};

class update_blank : public PatternBase{
public:
  void update();
};
