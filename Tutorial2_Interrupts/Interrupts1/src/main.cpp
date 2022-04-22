#include <Arduino.h>

#define BLUE    4 //LEDs
#define RED     5
#define GREEN   6
#define YELLOW  7

#define B_BLUE  8 //Butons
#define B_RED   9
#define B_GREEN 10
#define B_YELLOW 11

#define B_INTERRUPT 2 //debounced by 10uF

/* usage: 
  pressing the changing button will cause an interrupt and wait for 
  press of one of the other buttons, that will flip the appropriate LED state.
*/

bool B_BLUE_release = HIGH;
bool B_RED_release = HIGH;
bool B_GREEN_release = HIGH;
bool B_YELLOW_release = HIGH;

bool BLUE_state = LOW;
bool RED_state = LOW;
bool GREEN_state = LOW;
bool YELLOW_state = LOW;
volatile bool verbose_flag = LOW;
volatile bool change = LOW;


void flipLights();

void setup() {
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  pinMode(B_BLUE, INPUT_PULLUP);
  pinMode(B_RED, INPUT_PULLUP);
  pinMode(B_GREEN, INPUT_PULLUP);
  pinMode(B_YELLOW, INPUT_PULLUP);
  pinMode(B_INTERRUPT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(B_INTERRUPT), flipLights, FALLING );

  Serial.begin(9600);

}

void loop() {
  if (change == HIGH){ //change is triggered by an interrupt
    if (verbose_flag){
      Serial.println("Waiting for button press!");
      verbose_flag = LOW;
    }
    bool BLUE_read = !digitalRead(B_BLUE);
    bool RED_read = !digitalRead(B_RED);
    bool GREEN_read = !digitalRead(B_GREEN);
    bool YELLOW_read = !digitalRead(B_YELLOW);

    if (BLUE_read != B_BLUE_release) {
      B_BLUE_release = BLUE_read;
      if (BLUE_read) {
        BLUE_state = !BLUE_state;
        change = LOW;
      }
    }

    if (RED_read != B_RED_release) {
      B_RED_release = RED_read;
      if (RED_read) {
        RED_state = !RED_state;
        change = LOW;
      }
    }

    if (GREEN_read != B_GREEN_release) {
      B_GREEN_release = GREEN_read;
      if (GREEN_read) {
        GREEN_state = !GREEN_state;
        change = LOW;
      }
    }

    if (YELLOW_read != B_YELLOW_release) {
      B_YELLOW_release = YELLOW_read;
      if (YELLOW_read) {
        YELLOW_state = !YELLOW_state;
        change = LOW;
      }
    } 

    if (change == LOW){ //button pressed, before leaving, print and change colors
      Serial.println("changed color!\nDeactivating loop!");
      digitalWrite(BLUE, BLUE_state);
      digitalWrite(RED, RED_state);
      digitalWrite(GREEN, GREEN_state);
      digitalWrite(YELLOW, YELLOW_state);
      } 
    }
  
}

void flipLights() {
  //To avoid staying in the interrupt and waiting for the button press, 
  // we just indicate that something was change and let the main loop proccess it.
  change = HIGH;  
  verbose_flag = HIGH;
}