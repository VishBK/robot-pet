#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "motor_control.h"
#include "serial_com.h"

#define BAUD_RATE   9600    // 9600 bits/sec
#define DELAY       50      // Delay for coms


void setup() {
    Serial.begin(BAUD_RATE);    // Set up Serial library at the BAUD_RATE
    while (!Serial) {
        ;   // wait for serial port to connect
    }

    MotorInit();    // Initialize all four motors
}

void loop() {
    // SendData("Hello Pi");     // Send data (test) to the Raspberry Pi

    // Receive data from Raspberry Pi if it's sent
    if (Serial.available() > 0) {
        ReceiveData();        // problem
        char dir = GetDirection();
        uint8_t spd = GetSpeed();
        SetSpeed(spd);
        switch (dir) {
            case 'F' :
                Move(FORWARD);
                break;
            case 'B' :
                Move(BACKWARD);
                break;
            case 'S' :
                Move(RELEASE);
                break;
            default :
                Move(RELEASE);
                Serial.println("Invalid direction");
        }    
        // Serial.print("Direction: ");
        // Serial.println(dir);
        // Serial.print("Speed: ");
        // Serial.println(spd);
    }
    
    delay(DELAY);
}
