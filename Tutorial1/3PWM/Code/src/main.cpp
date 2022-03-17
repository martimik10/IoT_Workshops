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
void read_speed();
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
int Speed_step = 15; //dividible by 255: 1, 3, 5, 15

int rising = 0;
int falling = 255;

int LED_rising, LED_falling, LED_stalling;
int R, G, B;

int i = 0;
int changing_color = 1;
void loop(){

    //define on which cycle is what happening
    switch (changing_color)
    {
    case 1:
        LED_rising = GREEN;
        LED_falling = RED;
        LED_stalling = BLUE;// for staying at lowest value only
        R = falling;        //for printing purposes only
        G = rising;
        break;
    case 2:
        LED_rising = BLUE;
        LED_falling = GREEN;
        LED_stalling = RED;
        B = rising;
        G = falling;
        break;

    case 3: 
        LED_rising = RED;
        LED_falling = BLUE;
        LED_stalling = GREEN;
        R = rising;
        B = falling;
        break;
    
    default:
        break;
    }

    //light up!
    analogWrite(LED_rising, rising);
    analogWrite(LED_falling, falling);
    analogWrite(LED_stalling, LOW);

    //check for speed change, increment if possible, otherwise change cycle
    read_speed();
    if (i + Speed_step <= 255){ //if we are not in end of cycle, increment
        i += Speed_step;
    }
    else{ //we are in the end of cycle, reset i and switch to another cycle mode
        i = 0;
        changing_color == 3 ? changing_color = 1 : changing_color += 1;    
    }

    //print
    PRINT(R);
    PRINT(G);
    PRINT(B);
    PRINTLN(Speed_step);
    //update values
    rising = i ;
    falling = 255 - i;


}

void read_speed(){
    //check for new inputs
    if (Serial.available() > 0){
        
        int speed_input= Serial.read();

        switch (speed_input)
        {
        case '1':
            Speed_step = 1;
            break;
        case '2':
            Speed_step = 3;
            break;
        case '3':
            Speed_step = 5;
            break;
        case '4': 
            Speed_step = 15;
            break;
                
        default:
            Serial.println("Invalid speed! Use numbers 1 to 4");
            Speed_step = 1;
            break;
        }
    }
}
