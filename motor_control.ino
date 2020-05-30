/*
For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define NUM_MOTORS 4
#define MAX_MOTOR_SPEED 225

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Set motors
Adafruit_DCMotor* backLeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor* backRightMotor = AFMS.getMotor(2);
Adafruit_DCMotor* frontRightMotor = AFMS.getMotor(3);
Adafruit_DCMotor* frontLeftMotor = AFMS.getMotor(4);
Adafruit_DCMotor* motorArray[NUM_MOTORS] = {backLeftMotor, backRightMotor, frontRightMotor, frontLeftMotor};

void setup() {
    Serial.begin(9600);     // Set up Serial library at 9600 bps
    Serial.println("Motor test");
    
    AFMS.begin();       // Create with the default frequency 1.6KHz
    
    // Set motor speeds (0-255)
    uint8_t i;
    for (i = 0; i < NUM_MOTORS; i++) {
        motorArray[i]->setSpeed(0);
        motorArray[i]->run(RELEASE);
    }
}

void loop() {
    uint8_t i, j;

    // Turn all motors forwards
    for (i = 0; i < NUM_MOTORS; i++) {
        motorArray[i]->run(FORWARD);
    }

    for (i = 0; i < MAX_MOTOR_SPEED; i++) {
        for (j = 0; j < NUM_MOTORS; j++) {
            motorArray[j]->setSpeed(i);
        }
        delay(10);
    }
    delay(1000);
    for (i = MAX_MOTOR_SPEED; i > 0; i--) {
        for (j = 0; j < NUM_MOTORS; j++) {
            motorArray[j]->setSpeed(i);
        }
        delay(10);
    }

    delay(500);

    // Turn all motors backwards
    for (i = 0; i < NUM_MOTORS; i++) {
        motorArray[i]->run(BACKWARD);
    }

    for (i = 0; i < MAX_MOTOR_SPEED; i++) {
        for (j = 0; j < NUM_MOTORS; j++) {
            motorArray[j]->setSpeed(i);
        }
        delay(10);
    }
    delay(1000);
    for (i = MAX_MOTOR_SPEED; i > 0; i--) {
        for (j = 0; j < NUM_MOTORS; j++) {
            motorArray[j]->setSpeed(i);
        }
        delay(10);
    }

    delay(500);
}
