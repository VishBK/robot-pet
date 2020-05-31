/*
 * File to test serial communication between RPi and Arduino
*/

#include "serial_com.h"

void SendData() {
    Serial.println("Hello from Arduino!");
    delay(1000);
}
