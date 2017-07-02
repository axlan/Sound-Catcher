#include "SoundVisualizer.h"
#include "LEDController.h"
#include "LogBuffer.h"
#include "kiss_fftr.h"
#include "fix_fft.h"
#include <math.h>

  Value s_start_hue;
  Value s_stop_hue;

  Value s_brightness;

template <int NUM_LEVELS>
sound_base<NUM_LEVELS>::sound_base(): s_sample_offset(15),
                                      s_sample_scaling(2),
                                      s_sample_rate(16000),
                                      s_exp_scaling(float(9)),
                                      s_start_hue(160),
                                      s_stop_hue(0),
                                      s_brightness(64),
                                      last_exp_scaling(0) {
  add_val("input sample scaling", &s_sample_scaling);
  add_val("input sample offset", &s_sample_offset);
  add_val("sample rate", &s_sample_rate);
  add_val("fft exp scaling", &s_exp_scaling);
  add_val("start hue", &s_start_hue);
  add_val("stop hue", &s_stop_hue);
  add_val("brightness", &s_brightness);
}

template <int NUM_LEVELS>
void sound_base<NUM_LEVELS>::get_level_color(int level, CRGB &col) const
{
  uint8_t col_val;
  const uint8_t start = s_start_hue.get_int();
  const uint8_t stop = s_stop_hue.get_int();
  const int diff = stop - start;

  col_val = start + level * diff / (NUM_LEVELS - 1);

  hsv2rgb_rainbow(CHSV(col_val, 255, s_brightness.get_int()), col);
}

template <int NUM_LEVELS>
int16_t sound_base<NUM_LEVELS>::get_sample() const
{
  const unsigned int delayTime = 1000000/s_sample_rate.get_int();
  int16_t val = analogRead(A0) - 512;
  val = (val + s_sample_offset.get_int()) / s_sample_scaling.get_int();
  delayMicroseconds(delayTime);
  return val;
}


template <int NUM_LEVELS>
const uint16_t* sound_base<NUM_LEVELS>::get_levels()
{
  generate_levels();
  return levels;
}

template <int NUM_LEVELS>
int sound_base<NUM_LEVELS>::find_level(uint16_t val)
{
  generate_levels();
  for(int i = 0; i < NUM_LEVELS; i++) {
    if (val < levels[i]) {
      return i;
    }
  }
  return NUM_LEVELS - 1;
}

template <int NUM_LEVELS>
void sound_base<NUM_LEVELS>::generate_levels()
{
  if (last_exp_scaling == s_exp_scaling.get_float()) {
    return;
  }
  last_exp_scaling = s_exp_scaling.get_float();
  const float scale = 1;
  const float exp_step = s_exp_scaling.get_float() / float(NUM_LEVELS);
  
  for(int i = 0; i < NUM_LEVELS; i++) {
    levels[i] = scale * pow(2.0, exp_step * float(i));
  }
}


update_fft_test::update_fft_test():done(false){}

void update_fft_test::update() {
  if (!done) {
    const double samplingFrequency = 8000;
    const unsigned int delayTime = 1000000/samplingFrequency;
    const int samples = 32;
    const int8_t test[] = { 127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90 };
    
    {
      int8_t vReal[samples];
      int8_t vImag[samples];
      LogBuffer::Write("<b>fix_fft</b>");
      for (size_t i = 0; i < samples; i++)
      {
          vReal[i] = test[i];
      }
      memset(vImag, 0, sizeof(vImag));
      uint64_t start = micros();
      fix_fft(vReal, vImag, 5, 0);
      LogBuffer::Write(micros() - start);
      for(uint16_t i =0;i<samples/2;i++)
      {
        int power = vReal[i] * vReal[i] + vImag[i] * vImag[i];
        LogBuffer::Write(power);
      }
    }
      
    {
      LogBuffer::Write("<b>kiss_fftr</b>");

      kiss_fft_scalar vReal[samples];
      for (size_t i = 0; i < samples; i++)
      {
          vReal[i] = test[i];
      }
      kiss_fftr_cfg cfg = kiss_fftr_alloc(32, 0, NULL, NULL);

      kiss_fft_cpx freqdata[samples/2 + 1];

      uint64_t start = micros();
      kiss_fftr( cfg, vReal, freqdata);
      LogBuffer::Write(micros() - start);

      
      for (int i=0; i < samples/2;i++){
        int power = freqdata[i].r * freqdata[i].r + freqdata[i].i * freqdata[i].i;
        LogBuffer::Write(power);
      }

      kiss_fftr_free(cfg);
    }

    done =true;
  }

	delay(100);
}

void update_log_samples::update() {
  const double samplingFrequency = 8000;
  const unsigned int delayTime = 1000000/samplingFrequency;
  const int samples = 32;
  
  int8_t vReal[samples];
  int8_t vImag[samples];

	for(uint16_t i =0;i<samples;i++)
  {
    vReal[i] = analogRead(A0) - 512 + 15;
    vImag[i] = 0;
    delayMicroseconds(delayTime);
  }

  LogBuffer::Write("<b>Samples:</b>");

/*
  for(uint16_t i =0;i<samples;i++)
  {
    LogBuffer::Write(vReal[i]);
  }*/
  
  fix_fft(vReal, vImag, 5, 0);
  for(uint16_t i =0;i<samples/2;i++)
  {
    int power = int(vReal[i]) * int(vReal[i]) + int(vImag[i]) * int(vImag[i]);
    LogBuffer::Write(power);
  }
	delay(4000);
}

update_amplitude_star::update_amplitude_star()
{
  s_exp_scaling.set_float(20);
  s_sample_scaling.set_int(2);
}

void update_amplitude_star::update() {
  const uint16_t samples = 200;
  
  int power = 0;
  for(uint16_t i = 0; i < samples; i++)
  {
    power += abs(get_sample());
  }

  char msg[16];
  sprintf(msg, "p: %d", power);
  LogBuffer::Write(msg);
  
  int level = find_level(power);

  CRGB col;
  for(int i = 0; i < LedController::NUM_SPOKES; i++) {
    for (int k = 0; k <= LedController::NUM_LED_PER_SPOKE; k++) {
      if (level >= k) {
        get_level_color(k, col);
      } else {
        col = CRGB::Black;
      }
      LedController::setled(col, i, k);
    }
  }
  LedController::repaint();
  delay(100);
}

update_amplitude_basic::update_amplitude_basic(): s_ring_not_spoke(false) 
{ 
  s_exp_scaling.set_float(14);
  s_sample_scaling.set_int(2);
  add_val("ring not spoke", &s_ring_not_spoke);
}

void update_amplitude_basic::update() {

  const bool RINGS_NOT_SPOKES = s_ring_not_spoke.get_bool();
  const uint16_t samples = 200;
   
  int power = 0;
  for(uint16_t i = 0; i < samples; i++)
  {
    power += abs(get_sample());
  }

  char msg[16];
  sprintf(msg, "p: %d", power);
  LogBuffer::Write(msg);

  int lim1,lim2;


  if (RINGS_NOT_SPOKES) {
    lim1 = LedController::NUM_LED_PER_SPOKE;
    lim2 = LedController::NUM_SPOKES;
  } else {
    lim2 = LedController::NUM_LED_PER_SPOKE;
    lim1 = LedController::NUM_SPOKES;
  }

  const uint16_t* levels = get_levels();

  CRGB col;
  for (int i = 0; i < lim1; i++)
  {
    for(int j = 0; j < lim2; j++) {
      int idx = j + i * lim2;
      if (idx == 0 || power >= levels[idx]) {
        get_level_color(idx, col);
      } else {
        col = CRGB::Black;
      }
      if (RINGS_NOT_SPOKES) {
        LedController::setled(col, j, i);
      } else {
        LedController::setled(col, i, j);
      }
    }
  }
  LedController::repaint();

  delay(100);
}

uint16_t* fft32_base::run_fft()
{
  for(uint16_t i =0;i<samples;i++)
  {
    vReal[i] = get_sample();
    vImag[i] = 0;
  }
  fix_fft(vReal, vImag, 5, 0);
  for(uint16_t i =0;i<samples/2;i++)
  {
    mag_out[i] = vReal[i] * vReal[i] + vImag[i] * vImag[i];
  }
  return mag_out;
}

update_fft_spoke::update_fft_spoke(): s_hat_rate(float(.2)) {
  s_exp_scaling.set_float(9);
  s_sample_scaling.set_int(2);
  memset(spoke_hat, 0, sizeof(spoke_hat));
  add_val("hat fall rate", &s_hat_rate);
}

void update_fft_spoke::update()
{
  LedController::setall(CRGB::Black);
  uint16_t* mag = run_fft();

  
  CRGB col;
  for(int i = 0; i < LedController::NUM_SPOKES; i++) {

    spoke_hat[i] -= s_hat_rate.get_float();
    int level = find_level(mag[i]);
    spoke_hat[i] = std::max(spoke_hat[i], float(level));

    for (int k = 0; k <= level; k++)
    {
      get_level_color(k, col);
      LedController::setled(col, i, k);
    }
    hsv2rgb_rainbow(CHSV(0, 255, 64), col);
    LedController::setled(col, i, round(spoke_hat[i]));

  }
  
  LedController::repaint();
  
  delay(50);
}
