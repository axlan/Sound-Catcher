#include "SoundVisualizer.h"
#include "LEDController.h"
#include "LogBuffer.h"
#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */




void update_log_samples::update() {
    const double samplingFrequency = 8000;
    const unsigned int delayTime = 1000000/samplingFrequency;
    const uint16_t samples = 32; //This value MUST ALWAYS be a power of 2
    double vReal[samples];
    double vImag[samples];
    
	for(uint16_t i =0;i<samples;i++)
    {
        vReal[i] = double(analogRead(A0) - 512);
        delayMicroseconds(delayTime);
    }

    LogBuffer::Write("<b>Samples:</b>");
    char msg[16];

    for(uint16_t i =0;i<samples;i++)
    {
        sprintf(msg, "%d", (int)vReal[i]);
        LogBuffer::Write(msg);
    }

/*
    FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);	// Weigh data
    FFT.Compute(vReal, vImag, samples, FFT_FORWARD); // Compute FFT
    //FFT.ComplexToMagnitude(vReal, vImag, samples); // Compute magnitudes

    for (uint16_t i = 0; i < samples/2; i++)
    {
        int f = ((i * samplingFrequency) / samples);
        sprintf(msg, "%d %d", f, (int)sqrt(vReal[i]*vReal[i] + vImag[i]*vImag[i]));
        LogBuffer::Write(msg);
    }
*/
	delay(1000);
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
