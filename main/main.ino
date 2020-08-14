#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "motor_control.h"
#include "serial_com.h"
#include "distance_sensor.h"
#include "imu_sensor.h"

#define BAUD_RATE   9600    // 9600 bits/sec
#define DELAY       10      // Delay for serial in loop

#define STOP_DISTANCE   8       // Stop when an object is this far away in inches
#define DIST_DELAY      100     // milliseconds between each distance check
#define STOP_ANGLE      90      // Angle to turn by when running into object

bool isControlled;      // True if robot is manually controlled
bool loopSpdSet;        // flag to check if spd has previously been set in the loop
unsigned long prevTime; // stores the previous running time

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
    
    loopSpdSet = false;
    prevTime = 0;
    MotorInit();    // Initialize all four motors
    ImuInit();      // Initialize the IMU sensor
    PerformHandshake();
    Serial.println("Arduino handshake complete");
    isControlled = GetHandshake()[HANDSHAKE_LEN-1]-'0';   // Last char of handshake determines control
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
        Serial.print("Direction: ");
        Serial.println(dir);
        Serial.print("Speed: ");
        Serial.println(spd);
        SendData(ACK);
    }
    else if (Serial.peek() == HANDSHAKE_START[0] && bytesInSerial == HANDSHAKE_LEN) {
        // if another handshake is sent after confirming the first, reset Arduino
        if (CheckHandshake()) {
            delay(COM_DELAY);
            SendData(GetHandshake());
            isControlled = GetHandshake()[HANDSHAKE_LEN-1]-'0';
            FlushSerial();
            dir = 'S';
            spd = 0;
            Move(RELEASE);
            SendData(ACK);
        }
    }

    unsigned long curTime = millis();
    if (curTime - prevTime >= DIST_DELAY) {
        prevTime = curTime;
        long dist = GetDistance();

        // Check if anything is in front of bot
        if (dist <= STOP_DISTANCE && dir == 'F') {
            // if player controlled, then stop
            if (isControlled) {
                SetSpeed(0);
            
            // if not, then turn STOP_ANGLE degrees and continue moving forward
            } else {
                TurnToAngle(STOP_ANGLE);
                Move(FORWARD);
            }
            loopSpdSet = false;
        }
        else if (!loopSpdSet && dist > STOP_DISTANCE) {
            SetSpeed(spd);
            loopSpdSet = true;
        }
    }
    // delay(DELAY);
}
