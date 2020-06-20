#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "motor_control.h"
#include "serial_com.h"
#include "distance_sensor.h"

#define BAUD_RATE   9600    // 9600 bits/sec
#define DELAY       10      // Delay for serial in loop

#define STOP_DISTANCE   10      // Stop when an object is this far away in inches
#define IS_CONTROLLED   false   // True if robot is manually controlled

/**
 * Resets the Arduino back to setup()
*/
void (*resetFunc) (void) = 0;

/**
 * Called once initially by the Arduino compiler
*/
void setup() {
    while (!Serial) {
        ;   // wait for serial port to connect
    }
    Serial.begin(BAUD_RATE);    // Set up Serial library at the BAUD_RATE
    FlushSerial();

    MotorInit();    // Initialize all four motors
    PerformHandshake();
    Serial.println("\nArduino handshake complete");
    SendData(ACK);
}

/**
 * Called in a loop by the Arduino compiler
*/
void loop() {
    uint8_t bytesInSerial = Serial.available();

    // filter out carriage returns and new lines
    byte onTop = Serial.peek();
    while (onTop == CR || onTop == NL) {
        Serial.read();
        bytesInSerial = Serial.available();
        onTop = Serial.peek();
    }
    
    char dir;
    uint8_t spd;

    // Read data from the serial buffer when it reaches DATA_LENGTH+2 bytes (+2 for start and end chars)
    if (bytesInSerial >= DATA_LENGTH+2) {
        ReceiveData();
        ParseData();
        dir = GetDirection();
        spd = GetSpeed();
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
            case 'L' :
                Turn(-1);
                break;
            case 'R' :
                Turn(1);
                break;
            default :
                Move(RELEASE);
                Serial.println("Invalid direction");
        }
        // Serial.print("Direction: ");
        // Serial.println(dir);
        // Serial.print("Speed: ");
        // Serial.println(spd);
        SendData(ACK);
    }
    else if (Serial.peek() == HANDSHAKE[0] && bytesInSerial == HANDSHAKE_LEN) {
        // if another handshake is sent after confirming the first, reset Arduino
        if (CheckHandshake()) {
            delay(COM_DELAY);
            SendData(HANDSHAKE);
            FlushSerial();
            dir = 'S';
            spd = 0;
            Move(RELEASE);
            SendData(ACK);
        }
    }

    // Check if anything is in front of bot, if so, then turn right 90°
    long dist = GetDistance();
    if (dist < STOP_DISTANCE && dir == 'F') {
        SetSpeed(0);
    } else {
        SetSpeed(spd);
    }
    // delay(DELAY);
}
