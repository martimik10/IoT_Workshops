#include <Arduino.h>

#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)
#define BUTTON1 2
#define BUTTON2 7
#define LED 6
#define PATTERN 2 //USER ADJUSTABLE 1 or 2


void setup(){
    Serial.begin(9600);
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(LED, OUTPUT);
}

bool flag, flag2, flag3 = LOW; //Flag for first, second button and both_buttons pressed flags
bool LED_on = LOW;


void loop(){
    //if chosen PATTERN 1: no matter what switch is pressed, the LED changes state (from on to off, or from off to on)
    //if chosen PATTERN 2: when both switches are pressed at the same time, the LED changes state (from on to off, or from off to on)

    bool Button1_read = digitalRead(BUTTON1); //using PULLUP: if not pressed 1, if pressed 0
    bool Button2_read = digitalRead(BUTTON2);

    switch (PATTERN){
        case 1:
            if (Button1_read != flag) { //only go here once of full cycle PRESS+RELEASE
                flag = Button1_read;
                if (!Button1_read) {
                    LED_on = !LED_on;
                }
            }

            else if (Button2_read != flag2) {
                flag2 = Button2_read;
                if (!Button2_read) {
                    LED_on = !LED_on;  
                }
            }
            break;

        case 2:
            if ((Button1_read || Button2_read) != flag3) { // or gives 0 if both pressed, 1 otherwise (we're using pullup = not pressed = 1)
                flag3 = (Button1_read || Button2_read); //this continously flip the flag on opposite if both buttons are pressed

                if ((Button1_read || Button2_read) == LOW) { //if both pressed (and not released since)
                    LED_on = !LED_on;
                }
            }  
            break;
        
        default:
            break;
        }


    digitalWrite(LED, LED_on);
    PRINT(Button1_read);
    PRINT(Button2_read);
    PRINTLN(LED_on);
}
