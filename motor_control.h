
#define MAX_MOTOR_SPEED 255
#define NUM_MOTORS      4

void MotorInit();

void SetSpeed(uint8_t s);

void Move(uint8_t dir);

void Turn(int8_t dir);
