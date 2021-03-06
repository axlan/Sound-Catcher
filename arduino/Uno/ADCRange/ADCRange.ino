//http://apcmag.com/arduino-projects-digital-audio-recorder.htm/

/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/
// ----------------------------------------------------
//
// APC magazine - Arduino Masterclass
// Project #18 - Digital Audio Recorder v2.0
// Darren Yates - 11 April 2014
//
//
// ----------------------------------------------------

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

#include <FHT.h> // include the library



#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


unsigned long sampleRate = 22050;
unsigned int bufByteCount;
byte min_val;
byte max_val;

void new_run() {
  bufByteCount = 0;
  min_val = 255;
  max_val = 0;
  sbi (TIMSK2, OCIE2A); // enable timer interrupt, start grabbing audio
}

void setup() { // THIS RUNS ONCE
	
  Setup_timer2();
  Setup_ADC();
  
  Serial.begin(115200); // use the serial port
 
  new_run(); 
}

void loop() {
  if (bufByteCount < FHT_N) {
    return;
  }

  cbi (TIMSK2, OCIE2A); // disable timer interrupt
  /*
  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_log(); // take the output of the fht
  Serial.write(255); // send a start byte
  Serial.write(fht_log_out, FHT_N/2); // send out the data
  */

  Serial.print("min ");
  Serial.print(min_val);
  Serial.print(" max ");
  Serial.print(max_val);
  Serial.println();

  new_run();
}
  
void Setup_timer2() {

  TCCR2B = _BV(CS21);  // Timer2 Clock Prescaler to : 8
  TCCR2A = _BV(WGM21); // Interupt frequency  = 16MHz / (8 x 90 + 1) = 22191Hz
  OCR2A = 90; // Compare Match register set to 90

}

void Setup_ADC() {

  ADMUX = 0x60; // set ADC to read pin A0, ADLAR to 1 (left adjust), Vcc ref
  cbi(ADCSRA,ADPS2); // set prescaler to 8 / ADC clock = 2MHz
  sbi(ADCSRA,ADPS1);
  sbi(ADCSRA,ADPS0);
}

ISR(TIMER2_COMPA_vect) {
	
  sbi(ADCSRA, ADSC); // start ADC sample
  while(bit_is_set(ADCSRA, ADSC));  // wait until ADSC bit goes low = new sample ready

  byte new_val = ADCH;
  fht_input[bufByteCount] = new_val;
  bufByteCount++;
  
  if (new_val < min_val) {
    min_val = new_val;
  } else if (new_val > max_val) {
    max_val = new_val;
  }

}
