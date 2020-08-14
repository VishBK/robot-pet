#include "serial_com.h"

char receivedChars[DATA_LENGTH+1];
// char tempChars[DATA_LENGTH+1];
char handshake[HANDSHAKE_LEN+1];
char direction;   // Direction: F, B, S, L, or R
uint8_t speed;    // Speed: 0-255


void FlushSerial() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}


void SendData(char data[]) {
    Serial.println(data);
}

// Splits receivedChars on SEPERATOR, and assigns direction and speed
void ParseData() {
    char seperator[] = SEPERATOR;
    // strcpy(tempChars, receivedChars);    Copy if receivedChars is needed

    char* tokPtr = strtok(receivedChars, seperator);
    direction = tokPtr[0];
    tokPtr = strtok(NULL, seperator);
    speed = atoi(tokPtr);
}


void ReceiveData() {
    static bool isReceiving = false;
    static uint8_t idx = 0;
    uint8_t bytesRead = 0;
    char rc;
    
    /*
    * While there are bytes in the buffer,
    * assign receivedChars starting on START_DATA to END_DATA
    */
    while (Serial.available() > 0) {
        rc = Serial.read();
        bytesRead++;
        
        if (isReceiving == true) {
            if (rc != END_DATA) {
                receivedChars[idx] = rc;
                idx++;
                if (idx > DATA_LENGTH) {
                    idx = DATA_LENGTH;
                }
            }
            else {
                receivedChars[idx] = '\0'; // terminate the string
                isReceiving = false;
                idx = 0;
            }
        }
        else if (rc == START_DATA) {
            isReceiving = true;
        }
    }
}


bool CheckHandshake() {
    uint8_t idx = 0;
    char rc;
    while (Serial.available() > 0 && idx < HANDSHAKE_LEN) {
        rc = Serial.read();
        handshake[idx] = rc;
        idx++;
    }
    handshake[idx] = '\0';
    return strncmp(handshake, HANDSHAKE_START, HANDSHAKE_LEN-1) == 0;
}


void PerformHandshake() {
    bool handshakeComplete = false;
    while (!handshakeComplete) {
        handshakeComplete = CheckHandshake();
        SendData(handshake);
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


char* GetHandshake() {
    return handshake;
}
