#include <Arduino.h>


//With deep sleep, WiFi, System Clock and CPU is off. Only clock for deep sleep is active.
// NodeMCU ESP8266 draws only around 20uA. Modem-sleep draws 15mA and light sleeps 0.4mA.
// We physically connected D0 and RST with 1K resistor.

#define LED D7

float duty_cycle_ON = 0.2; // % can be 0.0 to 1.0
float duty_cycle_OFF = 1-duty_cycle_ON; // %
float full_cycle = 5; //seconds, can be floating point
int LED_ON_time = 100; //ms
int beep = 0;
float cycle_ON_sec = (full_cycle * duty_cycle_ON)*1e3;
unsigned long myTime;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  while(!Serial) { } //wait for Serial init
  //blink LED for duty cycle ON 100ms on 100ms off on the duty cycle 
  myTime = millis();
  Serial.println(myTime);


  while (beep < cycle_ON_sec) {
    Serial.println("BLINK");
    digitalWrite(LED, HIGH);
    delay(LED_ON_time);
    digitalWrite(LED, LOW);
    delay(LED_ON_time);

    beep += LED_ON_time*2;
  }
  //delay what was left from active duty cycle (or perform another task)
  // so duty cycle can be float
  beep -= LED_ON_time*2; //subtract as it overflew before beaking from while cycle
  delay(cycle_ON_sec - beep);

  Serial.println("Going to sleep!");
  ESP.deepSleep(full_cycle*duty_cycle_OFF*1e6); //Go to sleep for the remainder of cycle
}

void loop() {
  
}