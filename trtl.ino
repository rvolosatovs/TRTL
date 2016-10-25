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

int PULLER_SERVO_START = 25; 
int PULLER_SERVO_STOP = 150;

int GRABBER_SERVO_START = 150; 
int GRABBER_SERVO_FIRST = 80;
int GRABBER_SERVO_SECOND = 65;
int GRABBER_SERVO_STOP = 50;


int motor_left_enable = 10;
int motor_left_forward = 4;
int motor_left_backward = 7;

int motor_right_enable = 11;
int motor_right_forward = 12;
int motor_right_backward = 13;

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


    Serial.begin(9600); // in case of ethernet cable or WiFi
    /*Serial1.begin(115200); // in case of ethernet cable or WiFi*/

    // wait, give enough time for the communications processor to wake up
    /*WaitAndBlink(60000);*/

    // clear the input buffer
    while (Serial.available())
        Serial.read();  
}

void loop() {
    if(Serial.available() > 0) {

        char Command = Serial.read();

        switch(Command) {
            case 'h':
                // left
                digitalWrite(motor_left_forward, LOW);
                digitalWrite(motor_left_backward, HIGH);

                digitalWrite(motor_right_forward, HIGH);
                digitalWrite(motor_right_backward, LOW);
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

                digitalWrite(motor_right_forward, LOW);
                digitalWrite(motor_right_backward, HIGH);
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

            case 'r':
                grabber_down();
                break;

            case 't':
                grab();
                break;

            case 'y':
                pull();
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

void grabber_down() {
    puller_servo.attach(puller_servo_pin);
    grabber_servo.attach(grabber_servo_pin);

    puller_servo.write(PULLER_SERVO_START);
    delay(1500);
    grabber_servo.write(GRABBER_SERVO_FIRST);
    delay(100);
    grabber_servo.write(GRABBER_SERVO_START);
    delay(2000);

    puller_servo.detach();
    grabber_servo.detach();
}

int n = 0;
void grab() {
    n++;
    switch (n) {
        case 0:
            puller_servo.attach(puller_servo_pin);
            break;
        case 1:
            puller_servo.write(PULLER_SERVO_STOP);
            break;
        case 2:
            puller_servo.write(130);
            break;
        case 3:
            puller_servo.detach();
            break;
        case 4:
            grabber_servo.attach(grabber_servo_pin);
            break;
        case 5:
            grabber_servo.write(GRABBER_SERVO_START);
            break;
        case 6:
            grabber_servo.detach();
            n=0;
            break;
    }
}

void pull() {
    puller_servo.attach(puller_servo_pin);
    grabber_servo.attach(grabber_servo_pin);

    grabber_servo.write(GRABBER_SERVO_FIRST);
    delay(1000);
    puller_servo.write(PULLER_SERVO_STOP);
    delay(1500);
    grabber_servo.write(GRABBER_SERVO_SECOND);
    delay(500);
    grabber_servo.write(GRABBER_SERVO_STOP);
    delay(800);

    puller_servo.detach();
    grabber_servo.detach();
}
