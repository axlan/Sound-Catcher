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

#define BUFFER_SIZE 1024 // set to 256 point fht
uint8_t my_buffer[BUFFER_SIZE];

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


unsigned int bufByteCount;

void new_run() {
  bufByteCount = 0;
  sbi (TIMSK2, OCIE2A); // enable timer interrupt, start grabbing audio
}

void setup() { // THIS RUNS ONCE
	
  Setup_timer2();
  Setup_ADC();
  
  Serial.begin(115200); // use the serial port
 
  new_run(); 
}

void loop() {
   if (bufByteCount < BUFFER_SIZE) {
    return;
  }
  cbi (TIMSK2, OCIE2A); // disable timer interrupt

  Serial.println();
  for (int i = 0; i < BUFFER_SIZE; i++) {
    Serial.print(my_buffer[i]); // send out the data
    Serial.print(" ");
  }
  Serial.println();
  
  bufByteCount = 0;
  //new_run();
}
  
void Setup_timer2() {

  TCCR2B = _BV(CS21);  // Timer2 Clock Prescaler to : 8
  TCCR2A = _BV(WGM21); // Interupt frequency  = 16MHz / (8 x 90 + 1) = 22191Hz
  OCR2A = 90; // Compare Match register set to 90

}

void Setup_ADC() {

  //ADMUX = (1<<6) | (1<<5); // set ADC to read pin A0, ADLAR to 1 (left adjust), Vcc ref
  ADMUX = (1<<5); // set ADC to read pin A0, ADLAR to 1 (left adjust), AREF ref
  cbi(ADCSRA,ADPS2); // set prescaler to 8 / ADC clock = 2MHz
  sbi(ADCSRA,ADPS1);
  sbi(ADCSRA,ADPS0);
}

ISR(TIMER2_COMPA_vect) {
	
  sbi(ADCSRA, ADSC); // start ADC sample
  while(bit_is_set(ADCSRA, ADSC));  // wait until ADSC bit goes low = new sample ready

  
  my_buffer[bufByteCount] = ADCH;
  bufByteCount++;
  
}
