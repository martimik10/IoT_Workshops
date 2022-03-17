#include <Arduino.h>

//Already presented on Webex with prof. Fuertes with appropriate changes, unpacked from zip and transformed from .cpp to arduino format
//Change of code was made on request of mr. Fuertes to show how I would handle longer inputs. 
// Discussion was made on the downfalls and further possibilities of improvements of the script via Webex meeting.

//Im using VS Code Platformio extension instead of Arduino IDE so I need to include Arduino.h as well as declare functions in a proper C manner.
//please give me feedback on whether I should comment the code or not. Some lecturers want comments on every line, others hate them.

//Inspiration taken from https://create.arduino.cc/projecthub/thingerbits/9-led-patterns-with-arduino-277bf1

void knight_rider();
void loading();
void its_da_police();
void police_transition();

int repetitions = 4; //ADJUSTABLE BY USER
const unsigned int MAX_LENGHT = 12;

void setup() {
  Serial.begin(9600);
  Serial.print("\nPress 1 for Police mode\nPress 2 for Knigh Rider\nPress 3 for loading\n\n");

  for(int i = 8; i <= 13; i++){
  pinMode(i,OUTPUT);
 }
}

void loop(){
  //Initiate serial monitor read/write and let user choose which mode to briefly play selected mode. 
  //If started in Arduino IDE, send characters without 'newline'

  static char message[MAX_LENGHT];      //allocate space for message buffer
  static unsigned int position = 0;  
  unsigned int int_message;

  while (Serial.available() > 0){       //read keyboard number input until enter is hit
  
    char data = Serial.read();

    if (data != '\n' && (position < MAX_LENGHT-1)){
      message[position] = data;
      position++;
    }
    else{   //msg is successfully read
      message[position] = '\0'; //insert null character byte to show this is end of the buffer
      int_message = strtol(message, NULL, 10); //convert to int. Unusual characters will result in 0
      position = 0;

      switch (int_message){
        case 112:
          Serial.print("Police\n");
          its_da_police();
          break;
        
        case 22:
          Serial.print("Knight rider initiated\n");
          knight_rider();
          knight_rider();
          knight_rider();

          break;
        
        case 33:
          Serial.print("Loading pattern initiated\n");
          loading();
          loading();      
          break;

        default:
          Serial.print("\nInvalid key!! You pressed: ");
          Serial.print(int_message);
          Serial.print("\nPress 112 for Police mode\nPress 22 for Knigh Rider\nPress 33 for loading\n");
          break;
      }
    } 
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
