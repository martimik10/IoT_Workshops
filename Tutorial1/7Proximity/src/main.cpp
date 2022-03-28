#include <Arduino.h>

#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D3 Arduino to pin Trig of HC-SR04

#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)

#define LED1  7
#define LED2  8
#define LED3  9
#define LED4  10
#define LED5  11
#define BUZZ  12
#define STOP_BUTTON 4
#define MAX_DIST 50 //maximum distance (all leds shining)


#define NOTE_AS4 466
#define NOTE_D5  587
#define NOTE_C5  523
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_G4  392
#define NOTE_AS5 932
#define NOTE_A4  440
#define NOTE_A5  880
#define NOTE_F4  349

int melody[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, 
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, 
  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, 
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, 
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5
 };


// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
uint8_t LEDS_ON = 0;
uint8_t FLAG_LEDS_ON = 0;
int noteDuration = 750 / 4;
int pauseBetweenNotes = noteDuration * 1.30;
bool Button_release_flag = HIGH;
bool turnOffAlarm = LOW;
int thisNote = 0;

int th1 = 10;
int th2 = 20;
int th3 = 30;
int th4 = 40;
int th5 = 50;
int th1def = 10;
int th2def = 20;
int th3def = 30;
int th4def = 40;
int th5def = 50;
int HYST_margin = 1;

void setup() {
  for(int i = 7; i <= 12; i++){
    pinMode(i,OUTPUT);
  }

  pinMode(BUZZ, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(STOP_BUTTON, INPUT_PULLUP);

  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
}
void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1); //2
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH); //listens when in microseconds the wave came back
  // Calculating the distance
  distance = (duration / 2) / 29.1; //duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  PRINT(distance);
  PRINTLN(LEDS_ON);

  
  if (distance <= th1){
    if (LEDS_ON == 2)      th1 = th1def + HYST_margin;
    LEDS_ON = 1;

    bool BUTTON_read = !digitalRead(STOP_BUTTON);
    if (BUTTON_read != Button_release_flag){ //if button pressed, wait for its release
      Button_release_flag = BUTTON_read;
      if (BUTTON_read){
        turnOffAlarm = HIGH;
      }
    }
    if (!turnOffAlarm){ //if button was not pushed, play melody on repeat
      tone(12, melody[thisNote], noteDuration);
      delay(pauseBetweenNotes);
      noTone(12);

      thisNote < 96 ? thisNote++ : thisNote = 0; //If there is still alarm, loop through melody
    }

  }
  else if (distance <= th2){
    if (LEDS_ON == 1)         th1 = th1def - HYST_margin;
    if (LEDS_ON == 3)         th2 = th2def + HYST_margin;
    LEDS_ON = 2;
    thisNote = 0;
    turnOffAlarm = LOW;
  }
  else if (distance <= th3){
    if (LEDS_ON == 2)         th2 = th2def - HYST_margin;
    if (LEDS_ON == 4)         th3 = th3def + HYST_margin;
    LEDS_ON = 3;
    thisNote = 0;
    turnOffAlarm = LOW;
  }
  else if (distance <= th4){
    if (LEDS_ON == 3)         th3 = th3def - HYST_margin;
    if (LEDS_ON == 5)         th4 = th4def + HYST_margin;
    LEDS_ON = 4;
    thisNote = 0;
    turnOffAlarm = LOW;
  }
  else if (distance >=th5 && distance <= 4000){
    if (LEDS_ON == 4)         th4 = th4def - HYST_margin;
    LEDS_ON = 5;
    thisNote = 0;
    turnOffAlarm = LOW;
  }

  if (LEDS_ON != FLAG_LEDS_ON){ //play alarm only once we get close
  
    FLAG_LEDS_ON = LEDS_ON;
    for(int i = 7; i < 7 + LEDS_ON; i++){
      digitalWrite(i,HIGH);
    }
    
    for(int i = 12; i >= 7 + LEDS_ON; i--){
      digitalWrite(i, LOW);
    } 
  }
}