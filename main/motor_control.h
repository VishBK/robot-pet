#define MAX_MOTOR_SPEED 255
#define NUM_MOTORS      4

/**
 * Initialize the motors
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
 * Turns the bot
 * 
 * @param dir 1 for right, -1 for left
*/
void Turn(int8_t dir);

/**
 * Turns the bot until it reaches the given angle
 * 
 * @param angle integer from -360 to 360 to turn to 
*/
void TurnToAngle(uint16_t angle);
