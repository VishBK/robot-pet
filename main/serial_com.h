#define DATA_LENGTH     6
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


void FlushSerial();

void SendData(char data[]);

void ParseData();

void ReceiveData();

boolean CheckHandshake();

void PerformHandshake();

char GetDirection();

uint8_t GetSpeed();

char* GetReceivedChars();
