#include <Arduino.h>
#include <Wire.h>
#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)

void init_traffic();

//http://arduino.esp8266.com/stable/package_esp8266com_index.json
//SCL is on pin D1
//SDA is on pin D2
#define status_RED 1
#define status_RED_YELLOW 2
#define status_GREEN 3
#define status_YELLOW 4
#define status_YELLOW_BLINK 5
#define Slave_address_1 1
#define Slave_address_2 2
#define INT 2 //how many bytes we expect to receive (status + green request)
#define BUTTON 0

bool BUTTON_release = HIGH;
bool blinking_yellow_flag = HIGH;
byte status1 = 0;
byte status2 = 0;
byte Slave1_says = 0;
byte Slave2_says = 0;
byte Slave1_RQ_green = 0;
byte Slave2_RQ_green = 0;
int read_delay = 0;


void setup() {
  Serial.begin(9600); /* begin serial for debug */
  Wire.begin(); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  bool button_pressed = !digitalRead(BUTTON);
  if (button_pressed != BUTTON_release) {
    BUTTON_release = button_pressed;
    if (button_pressed) {
      switch (Slave1_says) {
        case status_RED:
          status1 = status_GREEN;
          status2 = status_RED;
          break;

        case status_GREEN:
          status1 = status_RED;
          status2 = status_GREEN;
          break;

        case status_YELLOW_BLINK:
          status1 = status_RED;
          status2 = status_GREEN;
          break;

        default:
          break;
      }
    }
  }

  if (read_delay >= 100000){
    read_delay = 0;
    Wire.requestFrom(Slave_address_1, INT);
    while (Wire.available()){
      Slave1_says = Wire.read();
      Slave1_RQ_green = Wire.read();
    } 

    Wire.requestFrom(Slave_address_2, INT);
    while (Wire.available()) {
      Slave2_says = Wire.read();
      Slave2_RQ_green = Wire.read();
    }

    if (blinking_yellow_flag == HIGH) {
      Serial.println("SENT init flag");
      //if both slaves say yellow blink, then turn set flag to low
      if ((Slave1_says == status_YELLOW_BLINK) && (Slave2_says == status_YELLOW_BLINK)) {
        blinking_yellow_flag = LOW;
      }
      status1 = status_YELLOW_BLINK;
      status2 = status_YELLOW_BLINK;
    }

    if (Slave1_RQ_green == 1 && Slave1_says != status_GREEN) {
      status1 = status_GREEN;
      status2 = status_RED;
    }

    if (Slave2_RQ_green == 1 && Slave2_says != status_GREEN) {
      status1 = status_RED;
      status2 = status_GREEN;
    }

    //if button is pressed once


    //if status1 is changed then write to slaves and its now blinking yellow
    //=wait for confirmation from slave of state changed, otherwise keep sending orders
    if (status1 != Slave1_says) {
      Wire.beginTransmission(Slave_address_1);
      Wire.write(status1);
      Wire.endTransmission();
      Serial.println("SENT status1");
    }

    if (status2 != Slave2_says) {
      Wire.beginTransmission(Slave_address_2);
      Wire.write(status2);
      Wire.endTransmission();
      Serial.println("SENT status2");

    }

      PRINT(status1);
      PRINT(status2);
      PRINT(Slave1_says);
      PRINT(Slave2_says);
      PRINT(Slave1_RQ_green);
      PRINT(Slave2_RQ_green);
      PRINTLN(blinking_yellow_flag);
  }
  read_delay++;
}
