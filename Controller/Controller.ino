/*
* PPM generator originally written by David Hasko
* on https://code.google.com/p/generate-ppm-signal/ 
*/

//pwm reading stuff 
#include <PinChangeInterrupt.h>
const byte channel_pin[] = {2,3,4,5,6};
const byte channel_value[] = {0,0,0,0,0};

volatile unsigned long rising_start[] = {0, 0, 0 ,0 ,0 };
volatile long channel_length[] = {0, 0, 0, 0, 0};

//////////////////////CONFIGURATION///////////////////////////////
#define CHANNEL_NUMBER 5  //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500  //set the default servo value
#define FRAME_LENGTH 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PULSE_LENGTH 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 10  //set PPM signal output pin on the arduino

/*this array holds the servo values for the ppm signal
change theese values in your code (usually servo values move between 1000 and 2000)*/
int ppm[CHANNEL_NUMBER];

void setup(){  
  //debugging
  Serial.begin(9600);

  //for pwm values.
  pinMode(channel_pin[0], INPUT);
  pinMode(channel_pin[1], INPUT);
  pinMode(channel_pin[2], INPUT);
  pinMode(channel_pin[3], INPUT);
  pinMode(channel_pin[4], INPUT);
  pinMode(channel_pin[5], INPUT);

  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[0]), onRising0, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[1]), onRising1, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[2]), onRising2, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[3]), onRising2, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[4]), onRising2, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channel_pin[5]), onRising2, CHANGE);


  //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
      ppm[i]= CHANNEL_DEFAULT_VALUE;
  }

  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)
  
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

}

void processPin(byte pin) {
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channel_pin[pin]));

  if(trigger == RISING) {
    rising_start[pin] = micros();
  } else if(trigger == FALLING) {
    channel_length[pin] = micros() - rising_start[pin];
  }
  //just making sure it fits with ppm max 2000 min 1000.
  if (channel_length[pin] > 2000)
  {
    channel_length[pin] = 2000;
  }else if (channel_length[pin] < 1000)
  {
    channel_length[pin] = 1000;
  }
  
}

void onRising0(void) {
  processPin(0);
}

void onRising1(void) {
  processPin(1);
}

void onRising2(void) {
  processPin(2);
}

void onRising3(void) {
  processPin(3);
}

void onRising4(void) {
  processPin(4);
}

void onRising5(void) {
  processPin(5);
}




void loop(){
  
  /*
    Here modify ppm array and set any channel to value between 1000 and 2000. 
    Timer running in the background will take care of the rest and automatically 
    generate PPM signal on output pin using values in ppm array
  */
  for (int i = 0; i < CHANNEL_NUMBER; i++)
  {
    ppm[i] = channel_length[i];

    //debugging
    Serial.print(ppm[i]);
    Serial.print(" | ");
  }
  Serial.readln("  ");
  
  
}






ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;// 
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
