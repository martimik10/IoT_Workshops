#include <Arduino.h>
#include <Wire.h>
#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)


//SCL is on pin A5
//SDA is on pin A4
#define Slave_address_2 2
#define status_RED 1
#define status_GREEN 3
#define status_YELLOW_BLINK 5
#define LED_GREEN   2
#define LED_YELLOW  3
#define LED_RED     4 
#define BUTTON_REQUEST_GREEN 5


byte read = 0;
byte status = 0;
byte status_RQ_green = 0;
byte newstatus = 0;
int print_delay = 0;
bool BUTTON_release = HIGH;


void setup() {
  Wire.begin(Slave_address_2);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUTTON_REQUEST_GREEN, INPUT_PULLUP);
}


void loop() {
  if (status != status_YELLOW_BLINK) {
    bool button_pressed = !digitalRead(BUTTON_REQUEST_GREEN);
    if (button_pressed != BUTTON_release) {
      BUTTON_release = button_pressed;
      if (button_pressed){
        status_RQ_green = 1;
      }
    }
  }


  if ((status == status_YELLOW_BLINK) || (status != newstatus)){
    status = newstatus;
    status_RQ_green = 0;
    switch (status) {
      case status_YELLOW_BLINK:
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_YELLOW, HIGH);
        delay(1000);
        digitalWrite(LED_YELLOW, LOW);
        delay(1000);
        Serial.println("switching to blink");
        break;
      case status_RED:
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_YELLOW, HIGH);
        delay(1000);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, HIGH);
        Serial.println("switching to red");

        break;
      case status_GREEN:
        //turn on yellow for one second, then turn off red and yellow and turn on green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_YELLOW, HIGH);
        delay(1000);
        digitalWrite(LED_YELLOW, LOW);
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        Serial.println("switching to green");

        break;
      default:
        break;
    }
  }
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
    read = Wire.read();
    PRINTLN(read);
    switch (read){
      case status_GREEN:
        Serial.println("received green");
        newstatus = status_GREEN;
        break;
      
      case status_RED:
        Serial.println("received red");
        newstatus = status_RED;
        break;
  
      case status_YELLOW_BLINK:
        Serial.println("received yellow blink");
        newstatus = status_YELLOW_BLINK;
        break;
  
      default:
        Serial.println("parsing problem!!!");
        break;
    }
}



// function that executes whenever data is requested from master
void requestEvent() {
  //print what was sent only each K cycles not to slowdown system
  if (print_delay == 200){
    Serial.print("sent to master: ");
    PRINT(status_RQ_green);
    PRINTLN(status);
    print_delay = 0;
  }
  Wire.write(status);
  Wire.write(status_RQ_green);

  print_delay++;
}