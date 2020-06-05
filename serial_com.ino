/*
 * File to test serial communication between RPi and Arduino
*/

#include "serial_com.h"

char receivedChars[DATA_LENGTH];
char direction;   // Direction: F, B, or R
uint8_t speed;    // Speed: 0-255


void FlushSerial() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}


void SendData(char data[]) {
    Serial.print(data);
}


void ParseData() {
    char seperator[] = ",";

    char* tokPtr = strtok(receivedChars, seperator);
    direction = tokPtr[0];
    tokPtr = strtok(NULL, seperator);
    speed = atoi(tokPtr);
}

// Modified from: https://forum.arduino.cc/index.php?topic=396450.0
void ReceiveData() {
    static boolean isReceiving = false;
    static uint8_t idx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    
    while (Serial.available() > 0) {
        rc = Serial.read();

        if (isReceiving == true) {
            if (rc != endMarker) {
                receivedChars[idx] = rc;
                idx++;
                if (idx >= DATA_LENGTH) {
                    idx = DATA_LENGTH - 1;
                }
            }
            else {
                receivedChars[idx] = '\0'; // terminate the string
                isReceiving = false;
                idx = 0;
            }
        }
        else if (rc == startMarker) {
            isReceiving = true;
        }
    }
    ParseData();
}


char GetDirection() {
    return direction;
}


uint8_t GetSpeed() {
    return speed;
}
