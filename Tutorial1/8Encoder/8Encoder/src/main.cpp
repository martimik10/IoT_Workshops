#include <Arduino.h>


//This cool definition also eliminates need for function which is more memorey/CPU efficient!
#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)
#define RED_LED     3
#define GREEN_LED   6
#define BLUE_LED    5
#define ENCODER_LED 13

#define RED_BUTTON  2
#define GREEN_BUTTON 7
#define BLUE_BUTTON 8
#define OFF_BUTTON  9
#define clkPin 10
#define dtPin 11
#define ENCODER_BUTTON 12
#define LEDS_off 0
#define ON  0
#define OFF 255 //remember we are using common cathode! 255 is off

/* Usage:
  Initialization:
    Encoder is inactive - turning doesn't do anything
    LEDS are off
  Usage:
    Press any of 3 buttons to turn on LED on mid brightness
    Pressing encoder activated turning to change brightness
    Pressing OFF button turns off all LEDs and encoder

    Program remembers the LED brightness, unless they were on minimum value 
      when OFF button was pressed, to prevent confusion of the user
 *  */


int clk = digitalRead(clkPin);
int dt  = digitalRead(dtPin);
void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(ENCODER_LED, OUTPUT);
  
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(OFF_BUTTON, INPUT_PULLUP);

  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);

  Serial.begin(9600);
  Serial.println("Serial Monitor Initialized\n Turn on LEDs ");
}




// byte brightness = OFF; //start on mid brightness
byte LED_ON = LEDS_off; //which LED to turn on
byte LED_array[3] = {RED_LED, GREEN_LED, BLUE_LED};
bool INIT = HIGH; //init colors first run
int Bright_arr[3] = {OFF, OFF, OFF};
int disp_brightness[3] = {}; //for displaying only
bool change = HIGH; //flag when OFF switch was hit. Initialize first to OFF as this is first HIGH
bool release_flag1, release_flag2, release_flag3, release_flag4, release_flag_encoder = HIGH; //send only one instruction per press (PRESS+RELEASE)
bool ENCODER_ACTIVE = LOW;
int step = 15; //ADJUSTABLE - dividible by 255

int clk = digitalRead(clkPin);
int dt  = digitalRead(dtPin);

void loop() {
  bool R_read =   !digitalRead(RED_BUTTON); //if button pressed, var will be 1
  bool G_read =   !digitalRead(GREEN_BUTTON);
  bool B_read =   !digitalRead(BLUE_BUTTON);
  bool OFF_read = !digitalRead(OFF_BUTTON);
  bool ENCODER_read = !digitalRead(ENCODER_BUTTON);

  

  if ((OFF_read != release_flag4)){ //turn off if button pressed or on init when flag is HIGH
    release_flag4 = OFF_read;
    if (OFF_read){
      change = HIGH;
      ENCODER_ACTIVE = LOW;
      LED_ON = LEDS_off;
      //Reset to mid value to prevent confusion what to do when pressing LED button again
    }
  }

  if (R_read != release_flag1){ //if one of the button pressed. LED shall be turned on and others off.
    release_flag1 = R_read;
      if (R_read){
        LED_ON = RED_LED;
        change = HIGH;
        // Serial.println("RED INTRUCTION SENT"); //DEBUG
      }
  }

  else if (G_read != release_flag2){ //if one of the button pressed. LED shall be turned on and others off.
      release_flag2 = G_read;
      if (G_read){
          LED_ON = GREEN_LED;
          change = HIGH;
          // Serial.println("GREEN INTRUCTION SENT"); //DEBUG
      }
  }

  else if (B_read != release_flag3){ //if one of the button pressed. LED shall be turned on and others off.
    release_flag3 = B_read;
      if (B_read){
        LED_ON = BLUE_LED;
        change = HIGH;
        // Serial.println("RED INTRUCTION SENT"); //DEBUG
      }
  }

  else if (ENCODER_read != release_flag_encoder){ //if encoder button pressed
    release_flag_encoder = ENCODER_read; //wait for release
      if (ENCODER_read && LED_ON != LEDS_off){ //if encoder button pressed and LED is on
        change = HIGH; //initiate that something has changed
        ENCODER_ACTIVE ? ENCODER_ACTIVE = LOW : ENCODER_ACTIVE = HIGH; //flip states
        if (ENCODER_ACTIVE){ //if encoder is active re-init its position
          clk = digitalRead(clkPin);
          dt  = digitalRead(dtPin);
        }
      }
  }
  
  if (ENCODER_ACTIVE){
    int newCLK = digitalRead(clkPin);
    int newDT = digitalRead(dtPin);
    for (int i=0; i < 1600; i++) asm volatile ("NOP");
    if ( (newCLK != clk) && newCLK == LOW ) { // CLK has changed from HIGH to LOW
      change = HIGH;
      if ( dt == LOW) {
        for (int i=0; i < 3; i++){ //change brightness of only selected LED
          if (LED_ON == LED_array[i]){
            if (Bright_arr[i] + step <= 255){ //remember we are using common cathode! 255 is off
              Bright_arr[i] += step;
            }else{
              Serial.println("Min Brightness Reached");
              Bright_arr[i] = 255;
            }
          }
        }
      }else {
        for (int i=0; i < 3; i++){
          if (LED_ON == LED_array[i]){
            if (Bright_arr[i] - step >= 0){
              Bright_arr[i] -= step;
            }else{
              Serial.println("Max Brightness Reached");
              Bright_arr[i] = 0;
            }
          }
        }
      }
    }
    clk = newCLK;
    dt = newDT;
  }

  if (change){
    change = LOW;
    //let the user know encoder is active by LED
    ENCODER_ACTIVE ? digitalWrite(ENCODER_LED, HIGH) : digitalWrite(ENCODER_LED, LOW); 
    
    if (INIT){
      for (int i=0; i < 3; i++){
        analogWrite(LED_array[i], Bright_arr[i]);
        Serial.println(Bright_arr[i]);
      }
      INIT = LOW;
    }

    if (LED_ON != LEDS_off) { //prevent writing on pin 0 as it is not connected
      for (int i=0; i < 3; i++){ //turn off/all other LEDs
        if (LED_ON == LED_array[i]){
          analogWrite(LED_array[i], Bright_arr[i]);
        }
      }
    }else{
      for (int i=0; i < 3; i++){ //turn off/all other LEDs
        Bright_arr[i] = OFF;
        analogWrite(LED_array[i], Bright_arr[i]);
      }
    }

    disp_brightness[0] = 255 - Bright_arr[0]; //for displaying only
    disp_brightness[1] = 255 - Bright_arr[1];
    disp_brightness[2] = 255 - Bright_arr[2];

    PRINT(disp_brightness[0]);
    PRINT(disp_brightness[1]);
    PRINT(disp_brightness[2]);
    PRINT(LED_ON);
    PRINTLN(ENCODER_ACTIVE);
    
  }
}