#include <Servo.h>

// pin number of on-board LED
int ledPin = 13;

void WaitAndBlink( unsigned long DeltaMilliSec){
    // wait DeltaMilliSec milliseconds, LED blinks as a sign of life
    // as time passes frequency inreases
    unsigned long DeltaT = 0;
    unsigned long TZero = millis(); //read start time
    while(DeltaT<DeltaMilliSec){
        DeltaT = millis()-TZero;      //compute elapsed time
        delay(500-400*DeltaT/DeltaMilliSec);
        digitalWrite(ledPin,LOW);
        delay(500-400*DeltaT/DeltaMilliSec);
        digitalWrite(ledPin,HIGH);
    } 
}  

Servo servoX;
Servo servoY;

// initial servo posititions in degrees
int posY = 90; 
int posX = 90;

// arduino pins used to connect servo control signals
int servoPinX = A0;
int servoPinY = A1;

// Pulse Width Modulation (PWM) pins
int PWM1 = 3;
int PWM2 = 5;
int PWM3 = 6;
int PWM4 = 11;

void setup() {
    analogWrite(PWM1,0);
    analogWrite(PWM2,0);
    analogWrite(PWM3,0);
    analogWrite(PWM4,0);

    servoX.attach(servoPinX);
    servoY.attach(servoPinY);

    servoX.write(posX);
    servoY.write(posY);

    Serial.begin(9600); // in case of USB connection
    //Serial1.begin(115200); // in case of ethernet cable or WiFi

    // wait, give enough time for the communications processor to wake up
    WaitAndBlink(60000);

    // clear the input buffer
    while (Serial.available())
        Serial.read();
    //while (Serial1.available())
    //   Serial1.read();  
}

void loop() {
    if(Serial.available() > 0)
        //if(Serial1.available() > 0)

    {
        char Command = Serial.read();
        //char Command = Serial1.read();
        switch(Command) {
            // Motors
            case 'o':
                // stop
                analogWrite(PWM1,0);
                analogWrite(PWM2,0);
                analogWrite(PWM3,0);
                analogWrite(PWM4,0);
                break;
            case ';':
                // backwards
                analogWrite(PWM1,255);
                analogWrite(PWM2,0);
                analogWrite(PWM3,255);
                analogWrite(PWM4,0);
                break;
            case 'p':
                // forwards
                analogWrite(PWM1,0);
                analogWrite(PWM2,255);
                analogWrite(PWM3,0);
                analogWrite(PWM4,255);
                break;
            case 'l':
                // left
                analogWrite(PWM1,255);
                analogWrite(PWM2,0);
                analogWrite(PWM3,0);
                analogWrite(PWM4,255);
                break;  
            case '\'':
                // right
                analogWrite(PWM1,0);
                analogWrite(PWM2,255);
                analogWrite(PWM3,255);
                analogWrite(PWM4,0);
                break;

                // Camera
            case 'w':
                // up
                posY = posY + 5;
                servoY.write(posY);
                break;
            case 's':
                // down
                posY = posY - 5;
                servoY.write(posY);
                break;
            case 'a':
                // left
                posX = posX + 5;
                servoX.write(posX);
                break;
            case 'd':
                // right
                posX = posX - 5;
                servoX.write(posX);
                break;
        }
    }
}
