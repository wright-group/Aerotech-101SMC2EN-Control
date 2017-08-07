#include <EEPROM.h>

// pins
int on_board_led = 13;
int clock_pin = 7;
int direction_pin = 6;
int limit_ccw = 3;
int limit_cw = 2;

// variables
long position_current = 0; // steps
float inchsperstep = 0.000033334;
int wait = 50; // microseconds between pulses
long remaining = 0;
int direction = 0;
volatile byte ccw_interrupted = LOW;
volatile byte cw_interrupted = LOW;
#define INPUT_SIZE 100  // TODO: make this a reasonable value
#define sep " "
char input[INPUT_SIZE + 1];
char code = '0';
char c_number = '0';
double number = 0.;

void setup() {
  // initiate pins and write outputs to logic LOW
  pinMode(on_board_led, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(direction_pin, OUTPUT); // The logic potential of this output controls which direction the stage moves
  digitalWrite(on_board_led, LOW);
  digitalWrite(clock_pin, LOW);
  digitalWrite(direction_pin, LOW);
  pinMode(limit_ccw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(limit_ccw), ccwInterrupt, FALLING);
  pinMode(limit_cw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(limit_cw), cwInterrupt, FALLING);
  // startup settings update.
  readEEPROM(); // sets the current position to the last saved position
  // initiate serial port communcation
  Serial.begin(57600);
  while (!Serial); // wait for port to connect
}

void loop() {
  if (remaining != 0) {
    digitalWrite(on_board_led, HIGH);
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(wait);
    digitalWrite(clock_pin, LOW);
    digitalWrite(on_board_led, LOW);
    remaining -= 1;
    position_current += direction;
  }
  if (ccw_interrupted) {
    remaining = 0;
    tellStagePosition(0.);
    ccw_interrupted = LOW;
  }
  if (cw_interrupted) {
    remaining = 0;
    tellStagePosition(250.);
    cw_interrupted = LOW;
  }
  delayMicroseconds(wait);
}

void ccwInterrupt() {
  ccw_interrupted = HIGH;
}

void cwInterrupt() {
  cw_interrupted = HIGH;
}

void echo() {
  String string = Serial.readString();
  string.trim();
  Serial.println(string);
}

void EEPROMWritelong(long address, long value) {
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long EEPROMReadlong(long address) {
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void getStagePosition() {
  Serial.println((float)position_current * 25.4 * inchsperstep, DEC);
}

void homeStage() {
  direction = -1;
  remaining = -1;
  digitalWrite(direction_pin, LOW);
}

void moveStage(double mm) {
  long position_desired_steps = (long)(mm / 25.4 / inchsperstep);
  long steps = position_desired_steps - position_current;
  remaining = abs(steps);
  if (steps > 0) {
    direction = 1;
    digitalWrite(direction_pin, HIGH);
  }
  else if (steps < 0) {
    direction = -1;
    digitalWrite(direction_pin, LOW);
  }
}

void readEEPROM() {
  position_current = EEPROMReadlong(0);
}

void serialEvent() {
  // read serial into input char array
  byte size = Serial.readBytesUntil('\n', input, INPUT_SIZE);
  input[size] = 0;
  // parse input
  char *code = strtok(input, sep);
  char *c_number = strtok(0, sep);
  // convert input
  number = atof(c_number);
  // do
  if (*code == 'M') moveStage(number);
  else if (*code == 'H') homeStage();
  else if (*code == 'G') getStagePosition();
  else if (*code == 'T') tellStagePosition(number);
  else if (*code == 'S') EEPROMWritelong(0, position_current); // Saves the current position (in steps) to EEPROM
  else if (*code == 'Q') query(); // Reads and prints the EEPROM position (in mm)
  else if (*code == 'E') echo();
  else delay(100);
  Serial.flush();
}

void serialEventRun(void) {
  if (Serial.available()) serialEvent();
}

static inline int8_t sign(int val) {
  if (val < 0) return -1;
  if (val == 0) return 0;
  return 1;
}

void tellStagePosition(double mm) {
  position_current = (long)(mm / 25.4 / inchsperstep);
  EEPROMWritelong(0, position_current);
}

void query() {
  if (remaining == 0) Serial.println('R');
  else Serial.println('B');
}

