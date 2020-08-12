#include "imu_sensor.h"

Adafruit_ISM330DHCX imu;

float accelData[DATA_DIM];  // x, y, z in m/s²
float gyroData[DATA_DIM];   // x, y, z in degrees
float curAngle[DATA_DIM];   // pitch, roll, yaw
float tempData;             // temperature from the sensor
unsigned long prevImuTime;  // keeps the previous time to calculate delta time


void ImuInit() {
    if (!imu.begin_SPI(LSM_CS, LSM_SCL, LSM_DO, LSM_SDA)) {
        Serial.println("Failed to find ISM330DHCX chip");
    }
    prevImuTime = 0;
}


void UpdateImu() {
    uint8_t i;
    sensors_event_t accel, gyro, temp;
    imu.getEvent(&accel, &gyro, &temp);

    /* Update the delta time to get accurate gyro readings */
    unsigned long curTime = millis();
    unsigned long dtime = curTime - prevImuTime;
    prevImuTime = curTime;

    accelData[0] = accel.acceleration.x;
    accelData[1] = accel.acceleration.y;
    accelData[2] = accel.acceleration.z;
    
    gyroData[0] = degrees(gyro.gyro.x);
    gyroData[1] = degrees(gyro.gyro.y);
    gyroData[2] = degrees(gyro.gyro.z);

    // Using a complementary filter
    double gyroWeight = 0.98;
    double accelWeight = 1.0-gyroWeight;
    
    float pitch_acc = degrees(atan2f(accelData[1], accelData[2]));
    curAngle[0] = (gyroWeight*(curAngle[0] + gyroData[0]) + accelWeight*pitch_acc)*(dtime/1000.);

    float roll_acc = degrees(atan2f(accelData[0], accelData[2]));
    curAngle[1] = (gyroWeight*(curAngle[1] + gyroData[1]) + accelWeight*roll_acc)*(dtime/1000.);

    // accelerometer cannot determine yaw
    curAngle[2] += (gyroData[2]*(dtime/1000.));

    tempData = temp.temperature;
}


void GetRelAngle(float angle[DATA_DIM]) {
    angle[0] = curAngle[0];
    angle[1] = curAngle[1];
    angle[2] = curAngle[2];
}
