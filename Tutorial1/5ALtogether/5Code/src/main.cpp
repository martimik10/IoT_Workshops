#include <Arduino.h>
/*
Constraints:
- Preserve brighnteness if changing colors
- Brightness dierection preserved 
- preserve brightness and direction after turning ON/OFF
*/

//This cool definition also eliminates need for function which is more memorey/CPU efficient!
#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)
#define RED_LED     3
#define GREEN_LED   6
#define BLUE_LED    5

#define RED_BUTTON  2
#define GREEN_BUTTON 7
#define BLUE_BUTTON 8
#define OFF_BUTTON  9

//uncomment this if RGB is in common anode:
#define ON  0
#define OFF 255


void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(OFF_BUTTON, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Serial Monitor Initialized\n Press buttons for change of colors or OFF state");
}



byte brightness = ON; //byte for mem saving (= uint8_t)
byte LED_ON = RED_LED; //which LED to turn on
bool OFF_flag = HIGH; //flag when OFF switch was hit
bool release_flag1, release_flag2, release_flag3, release_flag4 = HIGH; //send only one instruction per press (PRESS+RELEASE)
bool INIT_OFF = HIGH; //as no case is called first, make sure all the leds are off
int step = 15; //ADJUSTABLE - int, as it needs to be also negative for reverse


void loop() {
  bool R_read =   !digitalRead(RED_BUTTON); //if button pressed, var will be 1
  bool G_read =   !digitalRead(GREEN_BUTTON);
  bool B_read =   !digitalRead(BLUE_BUTTON);
  bool OFF_read = !digitalRead(OFF_BUTTON);

  /*
  Program starts with all of the LEDs off. If one of the 4 buttons are pressed, 
  only ONE instruction is sent to change the color, where the brighteness and direction
  of change is preserved. The fourth button turns everything off.

  There are release flags for each buttons in order for the program to work, 
  even if the user presses more buttons simoultinously (which is however not desired)

  To keep memory down, debug prints are turned off. Can be turned on by uncommenting //DEBUG
  */
  
  if (INIT_OFF){ //initial off
      INIT_OFF = LOW;
      analogWrite(RED_LED, OFF); //turn off everything, as OFF was pressed
      analogWrite(GREEN_LED, OFF);
      analogWrite(BLUE_LED, OFF);
      // Serial.println("INITIAL OFF"); //DEBUG
  }

  if (R_read != release_flag1){ //if one of the button pressed. LED shall be turned on and others off.
    release_flag1 = R_read;
      if (R_read){
        LED_ON = RED_LED;
        analogWrite(GREEN_LED, OFF);
        analogWrite(BLUE_LED, OFF);
        OFF_flag = LOW;
        // Serial.println("RED INTRUCTION SENT"); //DEBUG
      }
  }

  else if (G_read != release_flag2){ //if one of the button pressed. LED shall be turned on and others off.
      release_flag2 = G_read;
      if (G_read){
          LED_ON = GREEN_LED;
          analogWrite(RED_LED, OFF);
          analogWrite(BLUE_LED, OFF);
          OFF_flag = LOW;
          // Serial.println("GREEN INTRUCTION SENT"); //DEBUG
      }
  }

  else if (B_read != release_flag3){ //if one of the button pressed. LED shall be turned on and others off.
    release_flag3 = B_read;
      if (B_read){
        LED_ON = BLUE_LED;
        analogWrite(GREEN_LED, OFF);
        analogWrite(RED_LED, OFF);
        OFF_flag = LOW;
        // Serial.println("RED INTRUCTION SENT"); //DEBUG
      }
  }
  
  
  else if ((OFF_read != release_flag4)){ //turn off if button pressed or on init when flag is HIGH
    release_flag4 = OFF_read;
    if (OFF_read){
      OFF_flag = HIGH;
      analogWrite(RED_LED, OFF); //turn off everything, as OFF was pressed
      analogWrite(GREEN_LED, OFF);
      analogWrite(BLUE_LED, OFF);
      // Serial.println("OFF INTRUCTION SENT"); //DEBUG
    }
  }



  if (OFF_flag != HIGH){ //remember brighntness = change it only when not OFF
    brightness += step;

    if (brightness <= 0 || brightness >= 255) { //go up and down
      step = -step;
    }

    analogWrite(LED_ON, brightness); //turn on LED which button was pressed

  }

  //Last thing we want to print must be PRINTLN
  PRINT(brightness);    //DEBUG
  // PRINT(LED_ON);        //DEBUG
  // PRINT(LED_OFF1);     //DEBUG
  // PRINT(LED_OFF2);     //DEBUG
  // PRINT(step);          //DEBUG
  PRINTLN(OFF_flag);    //DEBUG
  
//This code consumes approx. 2600 bytes of flash and 280 bytes of RAM. Without DEBUG prints 

}