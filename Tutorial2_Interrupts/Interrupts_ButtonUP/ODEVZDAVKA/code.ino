#include <Arduino.h>
#include <avr/sleep.h>

#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)
#define BUTTON1 2 //debounced by 100uF 
#define BUTTON2 3 //debounced by 100uF
#define LED 4     // BLUE LED 
#define LED_SLEEPING 7 //to indicate whether arduino is sleeping
#define PATTERN 1 //USER ADJUSTABLE 1 or 2


void Sleep();
void Wake1();
void Wake2();

bool flag, flag2, flag3 = LOW; //Flag for first, second button and both_buttons pressed flags
bool LED_on = LOW;
volatile bool change1 = LOW;
volatile bool change2 = LOW;
bool Button1_read;
bool Button2_read;
int button_behaviour;

void setup(){
  Serial.begin(9600);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(LED_SLEEPING, OUTPUT); //to indicate sleeping
  pinMode(LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON1), Wake1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), Wake2, FALLING);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}



void loop(){
  //if chosen PATTERN 1: no matter what switch is pressed, the LED changes state (from on to off, or from off to on)
  //if chosen PATTERN 2: when both switches are pressed at the same time, the LED changes state (from on to off, or from off to on)
  if (change1 || change2){ //if interrupted we do something, otherwise arduino sleeps
    switch (PATTERN){
      case 1:
        if (change1){
          LED_on = !LED_on;
          change1 = LOW; //turn off change and ignore if second button pressed
        }
        else if (change2){
          LED_on = !LED_on;
          change2 = LOW;
        }
        break;

      case 2:
        // we still need to wait for the second button to be pressed so we read again
        Button1_read = digitalRead(BUTTON1);
        Button2_read = digitalRead(BUTTON2);
        
        if ((Button1_read || Button2_read) != flag3) { // or gives 0 if both pressed, 1 otherwise (we're using pullup = not pressed = 1)
          flag3 = (Button1_read || Button2_read); //this continously flip the flag on opposite if both buttons are pressed

          if ((Button1_read || Button2_read) == LOW) { //if both pressed (and not released since)
            LED_on = !LED_on;
            change1, change2 = LOW;
          }
        }  
        if (Button1_read && Button2_read){
          change1 = LOW;
          change2 = LOW;
        }
        break;
    
      default:
        break;
      }

    
    digitalWrite(LED, LED_on);
  }

  else{
    Serial.println("gonna sleep");
    Sleep();
  }
}

void Wake1(){
  Serial.println("fired1");
  // detachInterrupt(digitalPinToInterrupt(BUTTON1));
  sleep_disable();
  change1 = HIGH;
}

void Wake2(){
  Serial.println("fired2");
  // detachInterrupt(digitalPinToInterrupt(BUTTON2));
  sleep_disable();
  change2 = HIGH;
}

void Sleep(){
  //change1, change2 = LOW;
  sleep_enable();
  
  digitalWrite(LED_SLEEPING, LOW);
  delay(1000); //allow LED to be turned off


  sleep_cpu();
  Serial.println("woke up");
  digitalWrite(LED_SLEEPING, HIGH);

}
