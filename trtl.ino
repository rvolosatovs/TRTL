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

Servo camera_servo_X;
Servo camera_servo_Y;
int camera_pos_X = 90;
int camera_pos_Y = 90; 
int camera_servo_X_pin = 5;
int camera_servo_Y_pin = 6;

Servo grabber_servo;
Servo puller_servo;
int grabber_servo_pin= 3;
int puller_servo_pin= 9;

// Puller
int PULLER_SERVO_RETRACT = 120;
int PULLER_SERVO_CONTRACT = 25; 

// Grabber
int GRABBER_SERVO_UP = 150; 
int GRABBER_SERVO_MIDDLE = 80; 


int motor_left_enable = 10;
int motor_left_backward = 4;
int motor_left_forward = 7;

int motor_right_enable = 11;
int motor_right_backward = 12;
int motor_right_forward = 13;

int wifi = 0;

void setup() {
    camera_servo_X.attach(camera_servo_X_pin);
    camera_servo_X.write(camera_pos_X);

    camera_servo_Y.attach(camera_servo_Y_pin);
    camera_servo_Y.write(camera_pos_Y);

    digitalWrite(motor_left_enable, HIGH);
    digitalWrite(motor_left_forward, LOW);
    digitalWrite(motor_left_backward, LOW);

    digitalWrite(motor_right_enable, HIGH);
    digitalWrite(motor_right_forward, LOW);
    digitalWrite(motor_right_backward, LOW);


    if (wifi) {
        Serial1.begin(115200);

        WaitAndBlink(60000);

        while (Serial1.available())
            Serial1.read();  
    } else {
        Serial.begin(9600);

        while (Serial.available())
            Serial.read();  
    }
}

void loop() {
    int av;
    if (wifi)
        av = Serial1.available();
    else
        av = Serial.available();

    if(av > 0) {
        char c;
        if (wifi)
            c = Serial1.read();
        else
            c = Serial.read();

        switch(c) {
            case 'h':
                // left
                digitalWrite(motor_left_forward, LOW);
                digitalWrite(motor_left_backward, HIGH);

                digitalWrite(motor_right_backward, LOW);
                digitalWrite(motor_right_forward, HIGH);
                break;
            case 'j':
                // backwards
                digitalWrite(motor_left_forward, LOW);
                digitalWrite(motor_left_backward, HIGH);

                digitalWrite(motor_right_forward, LOW);
                digitalWrite(motor_right_backward, HIGH);
                break;
            case 'k':
                // forwards
                digitalWrite(motor_left_forward, HIGH);
                digitalWrite(motor_left_backward, LOW);

                digitalWrite(motor_right_forward, HIGH);
                digitalWrite(motor_right_backward, LOW);
                break;
            case 'l':
                // right
                digitalWrite(motor_left_forward, HIGH);
                digitalWrite(motor_left_backward, LOW);

                digitalWrite(motor_right_backward, HIGH);
                digitalWrite(motor_right_forward, LOW);
                break;  
            case 'n':
                // stop
                digitalWrite(motor_left_forward, LOW);
                digitalWrite(motor_left_backward, LOW);

                digitalWrite(motor_right_forward, LOW);
                digitalWrite(motor_right_backward, LOW);
                break;

            case 's':
                // left
                camera_pos_X += 5;
                camera_servo_X.write(camera_pos_X);
                break;
            case 'd':
                // down
                camera_pos_Y -= 5;
                camera_servo_Y.write(camera_pos_Y);
                break;
            case 'f':
                // up
                camera_pos_Y += 5;
                camera_servo_Y.write(camera_pos_Y);
                break;
            case 'g':
                // right
                camera_pos_X -= 5;
                camera_servo_X.write(camera_pos_X);
                break;

            case 'q':
                puller_retract();
                break;

            case 'w':
                puller_contract();
                break;

            case 't':
                grabber_middle();
                break;

            case 'y':
                grabber_up();
                break;

            case 'x':
                digitalWrite(ledPin, LOW);
                break;

            case 'c':
                digitalWrite(ledPin, HIGH);
                break;
        }
    }
}

void grabber_middle() {
    grabber_servo.attach(grabber_servo_pin);
    grabber_servo.write(GRABBER_SERVO_MIDDLE);
}

void grabber_up() {
    grabber_servo.attach(grabber_servo_pin);
    grabber_servo.write(GRABBER_SERVO_UP);
    delay(2000);
    grabber_servo.detach();
}

void puller_retract() {
    puller_servo.attach(puller_servo_pin);
    puller_servo.write(PULLER_SERVO_RETRACT);
    delay(2500);

    grabber_servo.attach(grabber_servo_pin);
    grabber_servo.write(45);
    delay(300);
    grabber_servo.detach();

    puller_servo.detach();
}

void puller_contract() {
    puller_servo.attach(puller_servo_pin);
    puller_servo.write(PULLER_SERVO_CONTRACT);

    delay(1000);
    int pm = millis();
    int cm = millis();
    grabber_servo.attach(grabber_servo_pin);
    while (cm - pm < 2000) {
        cm = millis();
        grabber_servo.write(60);
    }
    grabber_servo.detach();
    puller_servo.detach();
}

