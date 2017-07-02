#pragma once

#include "Patterns.h"
#include "LedController.h"


class update_fft_test : public PatternBase{
public:
  update_fft_test();
  void update();
private:
  bool done;
private:
  
};

class update_log_samples : public PatternBase{
public:
  void update();
private:
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

class fft32_base : public PatternBase{
protected:
  uint16_t* run_fft();

private:
  static const int samples = 32;
  int8_t vReal[samples];
  int8_t vImag[samples];
  uint16_t mag_out[samples/2];
};

class update_fft_spoke : public fft32_base{
public:
  update_fft_spoke();
  void update();

private:
  float spoke_hat[LedController::NUM_SPOKES];
  uint16_t levels[LedController::NUM_SPOKES];
  int find_level(uint16_t val);
  void generate_levels();
  Value s_exp_scaling;
	Value s_hat_rate;
};

