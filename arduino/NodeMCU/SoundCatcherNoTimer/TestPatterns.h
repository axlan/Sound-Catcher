#pragma once

#include "Patterns.h"

class update_red_1_led_test : public PatternBase{
private:
	int spoke_idx;
  int led_idx;
	int delay_ms;
public:
	update_red_1_led_test(int delay_ms);
	void update();
};

class update_red_8_led_test : public PatternBase{
private:
  int spoke_idx;
  int delay_ms;
public:
  update_red_8_led_test(int delay_ms);
  void update();
};

