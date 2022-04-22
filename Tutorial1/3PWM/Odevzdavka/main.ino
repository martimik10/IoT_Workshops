#include <Arduino.h>

//RGB PWM controller rainbow effect with adjustable speed and monitor output
// Define Pins
#define RED 3
#define GREEN 5
#define BLUE 6



//Program for smooth transition on RBG LED with implemented smooth transition between speeds defined by user keyboard: 1, 2, 3, 4.




//inspired by arduino.stackexchange.com/questions/9174/serial-print-string-and-variable-on-same-line
#define PRINT(var) Serial.print(#var ":\t"); Serial.print(var); Serial.print('\t')
#define PRINTLN(var) Serial.print(#var ":\t"); Serial.println(var)

//Function declarations
int printRGB(int redValue, int greenValue, int blueValue);

void setup(){
    Serial.begin(9600);
    Serial.println("\n RGB values of LED:\n");

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
}

char Speed_input = '4'; //ADJUSTABLE BY USER: 1, 2, 3
int Speed;              //in sake of nice printing we copy speed input here later
int Speed_step = 15; //dividible by 255: 1, 3, 5, 15
int redValue;
int greenValue;
int blueValue;


void loop(){
        //Inspired by Tutorial1, Moodle for this course:
    redValue = 255; // choose a value between 1 and 255 to change the color.
    greenValue = 0;
    blueValue = 0;
    analogWrite(RED, redValue);
    analogWrite(GREEN, greenValue);
    analogWrite(BLUE, blueValue);
    // delay(1000);
    int i = 0;
    while (i < 255 && i >= 0){
        //if we change to lower speed, i value will be remembered the same and will overflow the LED. 
            //this way, the LED wait on minimum value and vice versa in order to synchronize for new loop.
        redValue - Speed_step > Speed_step ?    redValue -= Speed_step      : redValue = 0;
        greenValue + Speed_step < 255 ?         greenValue += Speed_step    : greenValue = 255;
        // redValue -= Speed_step;
        // greenValue += Speed_step;
        analogWrite(RED, redValue);
        analogWrite(GREEN, greenValue);
        Speed_step = printRGB(redValue, greenValue, blueValue);
        i += Speed_step;
    }

    i = 0;
    redValue = 0;
    greenValue = 255;
    blueValue = 0;
    while (i < 255 && i >= 0){
        greenValue - Speed_step > Speed_step ?  greenValue -= Speed_step    : greenValue = 0;
        blueValue + Speed_step < 255 ?          blueValue += Speed_step     : blueValue = 255;
        // greenValue -= Speed_step;
        // blueValue += Speed_step;
        analogWrite(GREEN, greenValue);
        analogWrite(BLUE, blueValue);
        Speed_step = printRGB(redValue, greenValue, blueValue);
        i += Speed_step;
    }

    i = 0;
    redValue = 0;
    greenValue = 0;
    blueValue = 255;
    while (i < 255 && i >= 0){
        blueValue - Speed_step > Speed_step ?     blueValue -= Speed_step      : blueValue = 0;
        redValue + Speed_step < 255? redValue += Speed_step    : redValue = 255;
        // blueValue -= Speed_step;
        // redValue += Speed_step;
        analogWrite(RED, redValue);
        analogWrite(BLUE, blueValue);
        Speed_step = printRGB(redValue, greenValue, blueValue);
        i += Speed_step;
    }
}

int printRGB(int redValue, int greenValue, int blueValue){
    //check for new inputs
    if (Serial.available() > 0){
        
        int speed_input= Serial.read();

        switch (speed_input)
        {
        case '1':
            Speed = 1;
            Speed_step = 1;
            break;
        case '2':
            Speed = 2;
            Speed_step = 3;
            break;
        case '3':
            Speed = 3;
            Speed_step = 5;
            break;
        case '4': 
            Speed = 4;
            Speed_step = 15;
            break;
                
        default:
            Serial.println("Invalid speed! Use numbers 1 to 4");
            Speed = 1;
            Speed_step = 1;
            break;
        }
    }


    //Prints out R, G, B values in Serial monitor
    int R = redValue;
    int G = greenValue;
    int B = blueValue;

    PRINT(R);
    PRINT(G);
    PRINT(B);
    PRINTLN(Speed);
    return Speed_step;
}
