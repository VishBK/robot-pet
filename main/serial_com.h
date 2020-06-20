/*
 * File to test serial communication between RPi and Arduino
 * Data is sent to the Arduino in <D,sss> format where:
 *      D   = direction (F: forward, B: backward, S: release, L: left, R: right)
 *      sss = speed (000-255)
*/

#define DATA_LENGTH     5
#define NUM_ARGS        2
#define START_DATA      '<'
#define END_DATA        '>'
#define SEPERATOR       ","
#define COM_DELAY       500     // Delay for serial communication (ex: handshake)
#define HANDSHAKE       "@COM"
#define HANDSHAKE_LEN   4
#define ACK             "!"     // Acknowledge signal
#define CR              13      // Carriage return ASCII
#define NL              10      // New line ASCII

/**
 * Clears the incoming serial buffer
*/
void FlushSerial();

/**
 * Sends char array to outgoing serial buffer
 * 
 * @param data char array to send
*/
void SendData(char data[]);

/**
 * Parses incoming data into variables after calling ReceiveData() and receivedChars array is populated
 * Modifies receivedChars
*/
void ParseData();

/**
 * Receives valid, formatted data from the Serial buffer and assigns it to receivedChars
 * 
 * @see Modified from [original source](https://forum.arduino.cc/index.php?topic=396450.0)
*/
void ReceiveData();

/**
 * Reads from the serial buffer to see if the correct handshake is received
 * 
 * @return true if the serial buffer matches HANDSHAKE, false if incorrect
*/
bool CheckHandshake();

/**
 * Sends HANDSHAKE to the serial buffer every COM_DELAY ms until HANDSHAKE is received
 * Flushes serial buffer after receiving handshake
*/
void PerformHandshake();

/**
 * Returns current direction ('F', 'B', 'S', 'L', 'R')
 * 
 * @return char direction
*/
char GetDirection();

/**
 * Returns current speed (0-255)
 * 
 * @return int speed
*/
uint8_t GetSpeed();

/**
 * Returns receivedChars array
 * 
 * @return char array receivedChars
*/
char* GetReceivedChars();
