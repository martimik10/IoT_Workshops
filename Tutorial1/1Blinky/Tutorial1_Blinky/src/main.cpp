#include <Arduino.h>

//ALready presented on Webex with prof. Fuertes with appropriate changes, unpacked from zip and transformed from .cpp to arduino format

//Im using VS Code Platformio extension instead of Arduino IDE so I need to include Arduino.h as well as declare functions in a proper C manner.
//please give me feedback on whether I should comment the code or not. Some lecturers want comments on every line, others hate them.

//Inspiration taken from https://create.arduino.cc/projecthub/thingerbits/9-led-patterns-with-arduino-277bf1

void knight_rider();
void loading();
void its_da_police();
void police_transition();

int repetitions = 4; //ADJUSTABLE BY USER

void setup() {
 for(int i = 8; i <= 13; i++){
  pinMode(i,OUTPUT);
 }
}

void loop(){
  for(int i = 0; i <= repetitions; i++){
    its_da_police();
  }
  for(int i = 0; i <= repetitions; i++){
    knight_rider();
  }
  for(int i = 0; i <= repetitions; i++){
    loading();
  }
}

void knight_rider(){ 
  for(int i = 8; i <= 13; i++){
    digitalWrite(i,HIGH);
    delay(100);
    digitalWrite(i,LOW);
  }
  for(int i = 12; i >= 9; i--){
    digitalWrite(i,HIGH);
    delay(100);
    digitalWrite(i,LOW);
  }
}

void loading(){
  for(int i = 8; i <= 13; i++){
    digitalWrite(i,HIGH);
    delay(100);
  }
  for(int i = 13; i >= 8; i--){
    digitalWrite(i,LOW);
    delay(100);
  }
}

void its_da_police(){
  for (int y = 0; y <= 1; y++){

    for (int z = 0; z <=2; z++){
    digitalWrite(11,HIGH);
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    delay(100);
    }

    for (int z = 0; z <=2; z++){
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
    delay(50);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    delay(100);
    }
  }
  police_transition();
}

void police_transition(){
  for (int y = 0; y <= 1; y++){
    for (int i = 8; i <= 13; i = i + 2){
      digitalWrite(i, HIGH);
    }

    delay(150);

    for (int i = 8; i <= 13; i = i + 2){
      digitalWrite(i, LOW);
    }

    for (int i = 5; i <= 13; i = i + 2){
      digitalWrite(i, HIGH);
    }

    delay(150);

    for (int i = 5; i <= 13; i = i + 2){
      digitalWrite(i, LOW);
    }
  }
}

