#include <Adafruit_ISM330DHCX.h>

// For SPI mode, we need a CS pin
#define LSM_CS  10
// For software-SPI mode we need SCL/DO/SDA pins
#define LSM_SCL 13
#define LSM_DO  12
#define LSM_SDA 11
// Number of data dimensions for angle (pitch, roll, yaw)
#define DATA_DIM 3

/**
 * Initializes the IMU sensor using SPI
*/
void ImuInit();

/**
 * Updates the accelerometer, gyro, and temperature data from the IMU. Calculates the current angle of the IMU.
 * 
 * Uses weighted gyro data, accelerometer data and change in time in a 
 * <a href="https://www.pieter-jan.com/node/11">complementary filter</a> to calculate pitch, roll, and yaw.
 * Stores all captured data in respective arrays. Call this function whenever you need updated IMU values.
*/
void UpdateImu();

/**
 * Stores the pitch, roll, and yaw in angle[0], angle[1], and angle[2], respectively.
 * 
 * @param angle float array to store the current angles into
*/
void GetRelAngle(float angle[DATA_DIM]);
