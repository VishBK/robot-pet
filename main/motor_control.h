#define MAX_MOTOR_SPEED 255
#define NUM_MOTORS      4
#define IMU_TURN_DELAY  100 // time in ms to wait until updating the IMU when turning

/**
 * Initializes the motors
*/
void MotorInit();

/**
 * Sets the speed of the bot
 * 
 * @param s integer from 0-255
*/
void SetSpeed(uint8_t s);

/**
 * Moves the bot
 * 
 * @param dir FORWARD, BACKWARD, or RELEASE
*/
void Move(uint8_t dir);

/**
 * Turns the bot continuously
 * 
 * @param dir positive for right, negative for left
*/
void Turn(int8_t dir);

/**
 * Turns the bot relAngle from its current angle. 
 * Positive angle for clockwise, negative for counterclockwise
 * 
 * @param angle integer from -360 to 360 of the amount to turn
*/
void TurnToAngle(int16_t relAngle);
