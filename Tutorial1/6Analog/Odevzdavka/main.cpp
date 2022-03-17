#include <Arduino.h>

#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)

#define LDR_pin     A0
#define RED_LED     5
#define GREEN_LED   6
#define BLUE_LED    3
#define EMPTY_PIN   10

#define BUTTON1   7
#define BUTTON2   2

int LOW_th = 100; 
int HIGH_th = 600;
int LED_on = EMPTY_PIN; 
int LED_off1 = EMPTY_PIN;
int LED_off2 = EMPTY_PIN;


void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  Serial.begin(9600);


}



void loop() {
  //LDR READ
  int LRD_read = analogRead(LDR_pin);


  //THRESHOLDING
  bool set_threshold_LOW  =   !digitalRead(BUTTON1);
  bool set_threshold_HIGH =   !digitalRead(BUTTON2);

  if (set_threshold_LOW){
    LOW_th = LRD_read;
  }

  if (set_threshold_HIGH){
    HIGH_th = LRD_read;
  }

  //LED LOGIC
  if (LRD_read >= 0 && LRD_read < LOW_th){
    LED_on = BLUE_LED;
    LED_off1 = RED_LED;
    LED_off2 = GREEN_LED;
  }

  else if (LRD_read >= LOW_th && LRD_read < HIGH_th){
    LED_on = GREEN_LED;
    LED_off1 = RED_LED;
    LED_off2 = BLUE_LED;
  }

  else if (LRD_read >= HIGH_th && LRD_read < 1024){
    LED_on = RED_LED;
    LED_off1 = BLUE_LED;
    LED_off2 = GREEN_LED;
  }

  else{
    Serial.println("OUTTA BOUNDARIES");
  }

  digitalWrite(LED_on, HIGH);
  digitalWrite(LED_off1, LOW);
  digitalWrite(LED_off2, LOW);

  //DEBUG PRINTS
  PRINT(LOW_th);
  PRINT(HIGH_th);
  PRINT(LED_on);
  PRINTLN(LRD_read); 
}