#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "motor_control.h"
#include "serial_com.h"

#define BAUD_RATE   9600    // 9600 bits/sec

void setup() {
    Serial.begin(BAUD_RATE);    // Set up Serial library at the BAUD_RATE

    MotorInit();    // Initialize all four motors
}

void loop() {
    uint8_t i;
    SendData(); // Send data (test) to the Raspberry Pi

    // Accelerate motors forwards
    // Move(FORWARD);

    for (i = 0; i < MAX_MOTOR_SPEED; i++) {
        SetSpeed(i);
        delay(10);
    }
    delay(1000);
    for (i = MAX_MOTOR_SPEED; i > 0; i--) {
        SetSpeed(i);
        delay(10);
    }

    delay(500);

    // Accelerate motors backwards
    // Move(BACKWARD);

    for (i = 0; i < MAX_MOTOR_SPEED; i++) {
        SetSpeed(i);
        delay(10);
    }
    delay(1000);
    for (i = MAX_MOTOR_SPEED; i > 0; i--) {
        SetSpeed(i);
        delay(10);
    }

    delay(500);
}
