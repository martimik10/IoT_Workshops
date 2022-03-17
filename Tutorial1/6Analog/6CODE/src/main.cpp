#include <Arduino.h>

#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)

#define LDR_pin     A0
#define RED_LED     3
#define GREEN_LED   6
#define BLUE_LED    5

#define BUTTON1   2
#define BUTTON2   7

// uf using anode common RGB: else switch the numbers
#define ON  0
#define OFF 255

int LOW_th = 135; 
int HIGH_th = 420;
int LOW_th_default = LOW_th; 
int HIGH_th_default = HIGH_th;
int LED_on = GREEN_LED;
int lED_changed = LOW;
bool flag_init = LOW;
int HYST_margin = 30; //hysteresis for fluctuation
int value = 0;
bool release_flag1, release_flag2 = HIGH;

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  Serial.begin(9600);
}


void loop() {
  //////LDR READ
  int LRD_read = analogRead(LDR_pin);

  //////THRESHOLDING
  bool set_threshold_LOW  =   !digitalRead(BUTTON1);
  bool set_threshold_HIGH =   !digitalRead(BUTTON2);

  //////SET NEW THRESHOLD
  //Lower button sets lower threshold and second the higher one. If however low threshold is higher than the high threshold,
  // we will set this value as the high threshold instead.
  //Another feature could be not to let user change thresholds if they are too close enough
  //but its only another boring IFs and not implemented. 
  
  //set LOW
  if (set_threshold_LOW != release_flag1) { 
    release_flag1 = set_threshold_LOW; //we want to set this only once per PRESS + RELEASE

    if(set_threshold_LOW){
      value = analogRead(LDR_pin);
      if (value < HIGH_th){
        LOW_th = value;
        LOW_th_default = LOW_th;
        Serial.print("New LOW set! ");
        PRINT(LOW_th);
        PRINTLN(HIGH_th);
      }
      else{
        HIGH_th = value;
        HIGH_th_default = HIGH_th;
        Serial.print("New LOW would be higher than HIGH! Setting HIGH instead: ");
        PRINT(LOW_th);
        PRINTLN(HIGH_th);
      }
    }  
  }     



  //set HIGH
  if (set_threshold_HIGH != release_flag2){
    release_flag2 = set_threshold_HIGH;

    if (set_threshold_HIGH) {
      value = analogRead(LDR_pin);
      if (value > LOW_th){
        HIGH_th = value;
        HIGH_th_default = HIGH_th;
        Serial.print("New HIGH set! ");
        PRINT(LOW_th);
        PRINTLN(HIGH_th);
      }
      else{
        LOW_th = value;
        LOW_th_default = LOW_th;
        Serial.print("New HIGH would be lower than LOW! Setting LOW instead: ");
        PRINT(LOW_th);
        PRINTLN(HIGH_th);
      }
    }
  }  

  //User has set thresholds too close. Not implemented as in normal
  // usage this shouldn't happen. If it does, we atleast tell the user.
  if (HIGH_th_default - LOW_th_default <= HYST_margin){
    Serial.println("CAREFUL! The thresholds are too close!\nResetting to defaults!");
    LOW_th = 135; 
    HIGH_th = 420;
    LOW_th_default = LOW_th; 
    HIGH_th_default = HIGH_th;
    PRINT(LOW_th);
    PRINTLN(HIGH_th);
  }

  //////HYSTERESIS
  //Hysteresis is nice, but if light change is big enough, we don't need to worry about fluctuation anymore
  // so let's restart the threshold into the default values.
  if ((LRD_read >= LOW_th_default + HYST_margin) || (LRD_read <= LOW_th_default - HYST_margin)){
    LOW_th = LOW_th_default;

  }

  if ((LRD_read >= HIGH_th_default + HYST_margin) || (LRD_read <= HIGH_th_default - HYST_margin)){
    HIGH_th = HIGH_th_default;
  }

  //////LED LOGIC
  if (LRD_read < LOW_th && LED_on != BLUE_LED){
    //HYSTERESIS: if previous state was GREEN light, meaning we are going DOWN in brightness. Make it "harder" to switch back:
    if (LED_on == GREEN_LED)      LOW_th = LOW_th_default + HYST_margin; //If we continue going down with brightness, TH will be reset above

    LED_on = BLUE_LED;
    digitalWrite(BLUE_LED, ON);
    digitalWrite(RED_LED, OFF);
    digitalWrite(GREEN_LED, OFF);

    Serial.println("LOW");
  }

  else if (LRD_read >= LOW_th && LRD_read < HIGH_th && LED_on != GREEN_LED){
    //HYSTERESIS: now also implement if we went down from high value to middle OR from low to middle hysteresis
    if (LED_on == BLUE_LED)       LOW_th = LOW_th_default - HYST_margin;
    if (LED_on == RED_LED)        HIGH_th = HIGH_th_default + HYST_margin;

    LED_on = GREEN_LED;
    digitalWrite(GREEN_LED, ON);
    digitalWrite(RED_LED, OFF);
    digitalWrite(BLUE_LED, OFF);

    Serial.println("MEDIUM");

  }

  else if (LRD_read >= HIGH_th && LED_on != RED_LED){
    //HYSTERESIS
    if (LED_on == GREEN_LED)    HIGH_th = HIGH_th_default - HYST_margin;

    LED_on = RED_LED;
    digitalWrite(RED_LED, ON);
    digitalWrite(BLUE_LED, OFF);
    digitalWrite(GREEN_LED, OFF);

    Serial.println("HIGH");

  }


  

  //DEBUG PRINTS
  // PRINT(LOW_th);
  // PRINT(HIGH_th);
  PRINTLN(LRD_read); 
}