#include <Arduino.h>

#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D3 Arduino to pin Trig of HC-SR04

#define LED1  7
#define LED2  8
#define LED3  9
#define LED4  10
#define LED5  11
#define BUZZ  12
#define STOP_BUTTON 4
#define MAX_DIST 50 //maximum distance (all leds shining)



// #define NOTE_B0  31
// #define NOTE_C1  33
// #define NOTE_CS1 35
// #define NOTE_D1  37
// #define NOTE_DS1 39
// #define NOTE_E1  41
// #define NOTE_F1  44
// #define NOTE_FS1 46
// #define NOTE_G1  49
// #define NOTE_GS1 52
// #define NOTE_A1  55
// #define NOTE_AS1 58
// #define NOTE_B1  62
// #define NOTE_C2  65
// #define NOTE_CS2 69
// #define NOTE_D2  73
// #define NOTE_DS2 78
// #define NOTE_E2  82
// #define NOTE_F2  87
// #define NOTE_FS2 93
// #define NOTE_G2  98
// #define NOTE_GS2 104
// #define NOTE_A2  110
// #define NOTE_AS2 117
// #define NOTE_B2  123
// #define NOTE_C3  131
// #define NOTE_CS3 139
// #define NOTE_D3  147
// #define NOTE_DS3 156
// #define NOTE_E3  165
// #define NOTE_F3  175
// #define NOTE_FS3 185
// #define NOTE_G3  196
// #define NOTE_GS3 208
// #define NOTE_A3  220
// #define NOTE_AS3 233
// #define NOTE_B3  247
// #define NOTE_C4  262
// #define NOTE_CS4 277
// #define NOTE_D4  294
// #define NOTE_DS4 311
// #define NOTE_E4  330
// #define NOTE_F4  349
// #define NOTE_FS4 370
// #define NOTE_G4  392
// #define NOTE_GS4 415
// #define NOTE_A4  440
// #define NOTE_AS4 466
// #define NOTE_B4  494
// #define NOTE_C5  523
// #define NOTE_CS5 554
// #define NOTE_D5  587
// #define NOTE_DS5 622
// #define NOTE_E5  659
// #define NOTE_F5  698
// #define NOTE_FS5 740
// #define NOTE_G5  784
// #define NOTE_GS5 831
// #define NOTE_A5  880
// #define NOTE_AS5 932
// #define NOTE_B5  988
// #define NOTE_C6  1047
// #define NOTE_CS6 1109
// #define NOTE_D6  1175
// #define NOTE_DS6 1245
// #define NOTE_E6  1319
// #define NOTE_F6  1397
// #define NOTE_FS6 1480
// #define NOTE_G6  1568
// #define NOTE_GS6 1661
// #define NOTE_A6  1760
// #define NOTE_AS6 1865
// #define NOTE_B6  1976
// #define NOTE_C7  2093
// #define NOTE_CS7 2217
// #define NOTE_D7  2349
// #define NOTE_DS7 2489
// #define NOTE_E7  2637
// #define NOTE_F7  2794
// #define NOTE_FS7 2960
// #define NOTE_G7  3136
// #define NOTE_GS7 3322
// #define NOTE_A7  3520
// #define NOTE_AS7 3729
// #define NOTE_B7  3951
// #define NOTE_C8  4186
// #define NOTE_CS8 4435
// #define NOTE_D8  4699
// #define NOTE_DS8 4978

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
uint8_t noteDurations = 4;
bool FLAG_buzz = 0;
bool BUZZ_on = 0;


void setup() {
  for(int i = 7; i <= 12; i++){
    pinMode(i,OUTPUT);
  }

  pinMode(BUZZ, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT

  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
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
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 10){
    LEDS_ON = 1;
  }
  else if (distance <= 20){
    LEDS_ON = 2;
  }
  else if (distance <= 30){
    LEDS_ON = 3;
  }
  else if (distance <= 40){
    LEDS_ON = 4;
  }
  else if (distance <= 4000){
    LEDS_ON = 5;
  }

  if (LEDS_ON != FLAG_LEDS_ON){
    if (LEDS_ON == 1){

      for (int thisNote = 0; thisNote < 96; thisNote++) {

        int noteDuration = 750 / noteDurations;
        tone(12, melody[thisNote], noteDuration);

        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        
        noTone(12);
      }
    }


    FLAG_LEDS_ON = LEDS_ON;
    for(int i = 7; i < 7 + LEDS_ON; i++){
      Serial.print("ON");
      Serial.print(i);
      digitalWrite(i,HIGH);
    }
    
    for(int i = 12; i >= 7 + LEDS_ON; i--){
      Serial.print("OFF");
      Serial.println(i);
      digitalWrite(i, LOW);
    } 
  }

  //TODO Add hysteresis, add interrupts for alarm, add 4 button


  // digitalWrite(LED1, HIGH);
  // digitalWrite(LED2, HIGH);
  // digitalWrite(LED3, HIGH);
  // digitalWrite(LED4, HIGH);
  // digitalWrite(LED5, HIGH);

}