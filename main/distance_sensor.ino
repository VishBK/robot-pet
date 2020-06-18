#include <Wire.h>
#include "distance_sensor.h"


long Get_Distance() {
    /*
     * Scale factor is (Vcc/512) per inch. A 5V supply yields ~9.8mV/inArduino analog pin goes from 0 to 1024, 
     * so the value has to be divided by 2 to get the actual inches
    */
    return analogRead(DISTANCE_SENSOR_PIN) / 2;
}
