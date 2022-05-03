#include <Arduino.h>


//With deep sleep, WiFi, System Clock and CPU is off. Only clock for deep sleep is active.
// NodeMCU ESP8266 draws only around 20uA. Modem-sleep draws 15mA and light sleeps 0.4mA.
// We physically connected D0 and RST with 1K resistor.

#define LED D7

float duty_cycle_ON = 0.2; // % can be 0.0 to 1.0 (default 0.2)
float duty_cycle_OFF = 1-duty_cycle_ON; // %
float full_cycle = 4.9; //seconds, can be floating point (default 5)
int LED_ON_time = 100; //ms
int beep = 0;
float cycle_ON_sec = (full_cycle * duty_cycle_ON)*1e3;
unsigned long tic = 0;
unsigned long toc = 0;
unsigned long time_passed;
float result = 0;

void setup() {
  tic = 0;
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  while(!Serial) { } //wait for Serial init
  Serial.flush();
  //blink LED for duty cycle ON 100ms on 100ms off on the duty cycle 
  
  Serial.println("\nWaking up!");
  tic = millis();
  while (1) {
    toc = millis();
    //see is there is still head room for next cycle
    if (time_passed + LED_ON_time*2+5 < cycle_ON_sec){
      time_passed = toc-tic;
    }
    else{ 
      break;
      }
    
    Serial.println("BLINK");
    digitalWrite(LED, HIGH);
    delay(LED_ON_time);
    digitalWrite(LED, LOW);
    delay(LED_ON_time);
  }
  //delay what was left from active duty cycle (or perform another task)
  // so duty cycle can be float
  
  result = cycle_ON_sec - time_passed;
  if (result > 0) delay(result); //if the non-zero remainder due to float, wait for the ON cycle to finish

  
  ESP.deepSleep(full_cycle*duty_cycle_OFF*1e6); //Go to sleep for the remainder of cycle
}

void loop() {
  
}
