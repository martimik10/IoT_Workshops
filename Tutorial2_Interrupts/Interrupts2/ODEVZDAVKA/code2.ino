#include <Arduino.h>
#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)

/*TCNTx (Timer/CouNTer register), containing the actual value of the timer.
TCCRx (Timer/Counter Control Register), allowing to configure the prescaling value.
OCRx (Output Compare Register), for setting the counter value that should trigger the interrupt.
ICRx (Input Capture Register, only for 16bits timer1).
TIMSKx (Timer Interrupt MaSK register), to enable/disable timer interrupts.
TIFRx (Timer Interrupt Frag Register), indicating if there is a pending timer interrupt.
*/

#define BUTTON 2 //interrupt, debounced by capacitor 10uF
#define LED 4 //blue

volatile bool toggle1 = 0;
volatile int speed = 1;

void set_registers();

void setup() 
{
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  // put your setup code here, to run once:
  set_registers();
  attachInterrupt(digitalPinToInterrupt(BUTTON), set_registers, FALLING );
  
}

ISR(TIMER1_COMPA_vect){
  toggle1 = 1;
}

void loop() {
  if (toggle1) {
    toggle1 = 0;
    digitalWrite(LED, digitalRead(LED)^1);
  }
}

void set_registers(){
  noInterrupts(); //stop interrupts while configuring
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0; //initialize counter value to 0
  
  switch (speed){
    case 1:
      OCR1A = 62499; // = 16000000 / (256 * 1) - 1 (must be <65536)
      break;

    case 2:
      OCR1A = 31249; // = 16000000 / (256 * 2) - 1 (must be <65536)
      break;
    
    case 3:
      OCR1A = 20832; // = 16000000 / (256 * 3) - 1 (must be <65536)
      break;
    
    case 4:
      OCR1A = 62499; // = 16000000 / (64 * 4) - 1 (must be <65536)
      break;
    
    default:
      break;
  }

  TCCR1B |= (1 << WGM12);

  if (speed == 4)   TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10); //set correct prescale
  else              TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  
  interrupts(); //allow interrupts again

  speed == 4 ? speed = 1 : speed++; //cycle through speeds 1-4Hz
}
