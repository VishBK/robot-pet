/*
 * For use with the Adafruit Motor Shield v2 
 * ---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "motor_control.h"

Adafruit_DCMotor* motorArray[NUM_MOTORS];


void MotorInit() {
    // Create the motor shield object with the default I2C address
    Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

    // Set motors
    motorArray[0] = AFMS.getMotor(1);   // back left
    motorArray[1] = AFMS.getMotor(2);   // back right
    motorArray[2] = AFMS.getMotor(3);   // front right
    motorArray[3] = AFMS.getMotor(4);   // front left
    AFMS.begin();
}


void SetSpeed(uint8_t speed) {
    uint8_t i;
    for (i = 0; i < NUM_MOTORS; i++) {
        motorArray[i]->setSpeed(speed);
    }
}


void Move(uint8_t dir) {
    uint8_t i;
    for (i = 0; i < NUM_MOTORS; i++) {
        motorArray[i]->run(dir);
    }
}