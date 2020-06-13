/*
 * File to test serial communication between RPi and Arduino
*/

#include "serial_com.h"

char receivedChars[DATA_LENGTH];
// char tempChars[DATA_LENGTH];
char direction;   // Direction: F, B, S, R, or L
uint8_t speed;    // Speed: 0-255


void FlushSerial() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}


void SendData(char data[]) {
    Serial.println(data);
}


void ParseData() {
    char seperator[] = SEPERATOR;
    // strcpy(tempChars, receivedChars);    Copy if receivedChars is needed

    char* tokPtr = strtok(receivedChars, seperator);
    direction = tokPtr[0];
    tokPtr = strtok(NULL, seperator);
    speed = atoi(tokPtr);
}

// Modified from: https://forum.arduino.cc/index.php?topic=396450.0
void ReceiveData() {
    static boolean isReceiving = false;
    static uint8_t idx = 0;
    uint8_t bytesRead = 0;
    char startMarker = START_DATA;
    char endMarker = END_DATA;
    char rc;
    
    while (Serial.available() > 0) {
        rc = Serial.read();
        bytesRead++;
        
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
}


boolean CheckHandshake() {
    char received[HANDSHAKE_LEN+1];
    uint8_t idx = 0;
    char rc;
    while (Serial.available() > 0 && idx < HANDSHAKE_LEN) {
        rc = Serial.read();
        received[idx] = rc;
        idx++;
    }
    received[idx] = '\0';
    return strcmp(received, HANDSHAKE) == 0;
}


void PerformHandshake() {
    boolean handshakeComplete = false;
    while (!handshakeComplete) {
        handshakeComplete = CheckHandshake();
        SendData(HANDSHAKE);
        delay(COM_DELAY);
    }
    FlushSerial();
}


char GetDirection() {
    return direction;
}


uint8_t GetSpeed() {
    return speed;
}


char* GetReceivedChars() {
    return receivedChars;
}
