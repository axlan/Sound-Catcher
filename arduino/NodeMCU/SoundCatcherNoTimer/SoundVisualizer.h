#pragma once

#include "Patterns.h"
#include "LedController.h"

class update_log_samples : public PatternBase{
public:
  void update();
};

class update_amplitude_star : public PatternBase{
public:
  void update();
};


class update_amplitude_basic : public PatternBase{
public:
  update_amplitude_basic();
  void update();
private:
    static const int num_levels = LedController::NUM_LED_PER_SPOKE * LedController::NUM_SPOKES;
    int levels[num_levels];
};

