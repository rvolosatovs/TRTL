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

Servo grabber_arm_servo;
Servo grabber_pull_servo;
bool arm_lowered = true;
bool puller_contracted = true;
int grabber_arm_servo_pin= 3;
int grabber_pull_servo_pin= 9;
int GRABBER_PULL_SERVO_START = 60; 
int GRABBER_PULL_SERVO_STOP = 120;
int GRABBER_ARM_SERVO_START = 60; 
int GRABBER_ARM_SERVO_STOP = 120;

int motor_A_enable = 10;
int motor_A_direction_A = 4;
int motor_A_direction_B = 7;

int motor_B_enable = 11;
int motor_B_direction_A = 12;
int motor_B_direction_B = 13;

// Motors = 10,11 PWM A/B
// Directions = 4,7  mot. A
// Directions = 12,13 mot. B

void setup() {
    camera_servo_X.attach(camera_servo_X_pin);
    camera_servo_X.write(camera_pos_X);

    camera_servo_Y.attach(camera_servo_Y_pin);
    camera_servo_Y.write(camera_pos_Y);

    grabber_arm_servo.attach(grabber_arm_servo_pin);
    grabber_arm_servo.write(GRABBER_ARM_SERVO_START);

    grabber_pull_servo.attach(grabber_pull_servo_pin);
    grabber_pull_servo.write(GRABBER_PULL_SERVO_START);

    digitalWrite(motor_A_enable, HIGH);
    digitalWrite(motor_A_direction_A, LOW);
    digitalWrite(motor_A_direction_B, LOW);

    digitalWrite(motor_B_enable, HIGH);
    digitalWrite(motor_B_direction_A, LOW);
    digitalWrite(motor_B_direction_B, LOW);


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
    if(Serial.available() > 0) {
        /*if(Serial1.available() > 0) {*/

        char Command = Serial.read();
        //char Command = Serial1.read();

        switch(Command) {
            /**
             *   Motors
             */
            case 'o':
                // stop
                digitalWrite(motor_A_direction_A, LOW);
                digitalWrite(motor_A_direction_B, LOW);

                digitalWrite(motor_B_direction_A, LOW);
                digitalWrite(motor_B_direction_B, LOW);
                break;
            case ';':
                // backwards
                digitalWrite(motor_A_direction_A, HIGH);
                digitalWrite(motor_A_direction_B, LOW);

                digitalWrite(motor_B_direction_A, LOW);
                digitalWrite(motor_B_direction_B, HIGH);
                break;
            case 'p':
                // forwards
                digitalWrite(motor_A_direction_A, LOW);
                digitalWrite(motor_A_direction_B, HIGH);

                digitalWrite(motor_B_direction_A, HIGH);
                digitalWrite(motor_B_direction_B, LOW);
                break;
            case 'l':
                // left
                digitalWrite(motor_A_direction_A, HIGH);
                digitalWrite(motor_A_direction_B, LOW);

                digitalWrite(motor_B_direction_A, HIGH);
                digitalWrite(motor_B_direction_B, LOW);
                break;  
            case '\'':
                // right
                digitalWrite(motor_A_direction_A, LOW);
                digitalWrite(motor_A_direction_B, HIGH);

                digitalWrite(motor_B_direction_A, LOW);
                digitalWrite(motor_B_direction_B, HIGH);
                break;

                /**
                 *   Camera
                 */
            case 'w':
                // up
                camera_pos_Y += 5;
                camera_servo_Y.write(camera_pos_Y);
                break;
            case 's':
                // down
                camera_pos_Y -= 5;
                camera_servo_Y.write(camera_pos_Y);
                break;
            case 'a':
                // left
                camera_pos_X += 5;
                camera_servo_X.write(camera_pos_X);
                break;
            case 'd':
                // right
                camera_pos_X -= 5;
                camera_servo_X.write(camera_pos_X);
                break;
                /**
                 *   Grabber
                 */

            case 'e':
                grabber_arm_servo.write(GRABBER_ARM_SERVO_STOP);
                grabber_pull_servo.write(GRABBER_PULL_SERVO_STOP);
                break;
            case 'r':
                grabber_arm_servo.write(GRABBER_ARM_SERVO_START);
                grabber_pull_servo.write(GRABBER_PULL_SERVO_START);
                break;
        }
    }
}
