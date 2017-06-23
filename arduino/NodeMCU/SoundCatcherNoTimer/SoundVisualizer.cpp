#include "SoundVisualizer.h"
#include "LEDController.h"
#include "LogBuffer.h"
#include "kiss_fftr.h"
#include "fix_fft.h"

update_fft_test::update_fft_test():done(false){}

void update_fft_test::update() {
  if (!done) {
    const double samplingFrequency = 8000;
    const unsigned int delayTime = 1000000/samplingFrequency;
    const int samples = 32;
    int8_t vReal[samples];
    int8_t vImag[samples];
    const int8_t test[] = { 127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90 };
    
    {
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
      const int8_t test[] = { 127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90,127,90,0,-90,-127,-90,0,90 };
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
  static const int samples = 32;
  
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


#define TOP_POWER 5.0
#define SCALE 90.0
#define OFFSET 500.0

const int POWER_LEVELS[] = {0,
                            OFFSET,
                            SCALE * pow(2.0, TOP_POWER / 6.0 * 1.0) + OFFSET,
                            SCALE * pow(2.0, TOP_POWER / 6.0 * 2.0) + OFFSET,
                            SCALE * pow(2.0, TOP_POWER / 6.0 * 3.0) + OFFSET,
                            SCALE * pow(2.0, TOP_POWER / 6.0 * 4.0) + OFFSET,
                            SCALE * pow(2.0, TOP_POWER / 6.0 * 5.0) + OFFSET,
                            SCALE * pow(2.0, TOP_POWER / 6.0 * 6.0) + OFFSET};

void update_amplitude_star::update() {
   
  const double samplingFrequency = 8000;
  const unsigned int delayTime = 1000000/samplingFrequency;
  const uint16_t samples = 200; //This value MUST ALWAYS be a power of 2

  int power = 0;
  for(uint16_t i = 0; i < samples; i++)
  {
    //power += log(float(abs(analogRead(A0) - 512)));
    power += max(abs(analogRead(A0) - 512) - 15, 0);
    delayMicroseconds(delayTime);
  }

  char msg[16];
  sprintf(msg, "p: %d", power);
  LogBuffer::Write(msg);

  CRGB col;
  for (int i = 0; i < LedController::NUM_LED_PER_SPOKE; i++)
  {
    if (i == 0 || power >= POWER_LEVELS[i]) {
      uint8_t col_val = 96 - i * 96 / (LedController::NUM_LED_PER_SPOKE - 1);
      hsv2rgb_rainbow(CHSV(col_val, 255, 64), col);
      sprintf(msg, "l: %d %d", POWER_LEVELS[i], col_val);
      LogBuffer::Write(msg);
    } else {
      hsv2rgb_rainbow(CHSV(0, 0, 0), col);
    }
    LedController::setring(col, i);
  }
  LedController::repaint();

  delay(100);
}

update_amplitude_basic::update_amplitude_basic()
{ 
  const float scale = 10;
  const float top_exp = 8;
  const float exp_step = top_exp / float(num_levels);
  
  for(int i = 0; i < num_levels; i++) {
  levels[i] = scale * pow(2.0, exp_step * float(i));
  }
}

void update_amplitude_basic::update() {

  const bool RINGS_NOT_SPOKES = false;
   
  const double samplingFrequency = 8000;
  const unsigned int delayTime = 1000000/samplingFrequency;
  const uint16_t samples = 200; //This value MUST ALWAYS be a power of 2
   
  int power = 0;
  for(uint16_t i = 0; i < samples; i++)
  {
    //power += log(float(abs(analogRead(A0) - 512)));
    //power += max(abs(analogRead(A0) - 512) - 15, 0);
    power += abs(analogRead(A0) - 512 + 15);
    delayMicroseconds(delayTime);
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

  CRGB col;
  for (int i = 0; i < lim1; i++)
  {
    for(int j = 0; j < lim2; j++) {
      int idx = j + i * lim2;
      if (idx == 0 || power >= levels[idx]) {
        uint8_t col_val = 160 - float(idx) * 160. / float(num_levels);
        hsv2rgb_rainbow(CHSV(col_val, 255, 64), col);
      } else {
        hsv2rgb_rainbow(CHSV(0, 0, 0), col);
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
