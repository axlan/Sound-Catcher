#pragma once

#include "Patterns.h"
#include "LedController.h"


template <int NUM_LEVELS>
class sound_base : public PatternBase{
protected:
  
  void get_level_color(int level, CRGB &col) const;
  sound_base();
  int16_t get_sample() const;
  int find_level(uint16_t val);
  const uint16_t* get_levels();

  Value s_sample_scaling;
	Value s_sample_offset;
  Value s_sample_rate;
  
  Value s_exp_scaling;

  Value s_start_hue;
  Value s_stop_hue;

  Value s_brightness;

  private:
   float last_exp_scaling;
   void generate_levels();
   uint16_t levels[NUM_LEVELS];
};


class update_fft_test : public PatternBase{
public:
  update_fft_test();
  void update();
private:
  bool done;
  
};

class update_log_samples : public PatternBase{
public:
  void update();
private:
};

class update_amplitude_star : public sound_base<LedController::NUM_LED_PER_SPOKE> {
public:
  update_amplitude_star();
  void update();
};


class update_amplitude_basic : 
  public sound_base<LedController::NUM_LED_PER_SPOKE * LedController::NUM_SPOKES> {
public:
  update_amplitude_basic();
  void update();
private:
  Value s_ring_not_spoke;
};

class fft32_base : public sound_base<LedController::NUM_LED_PER_SPOKE> {
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
	Value s_hat_rate;
};
