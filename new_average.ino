#include <Arduino.h>
#include <SPI.h>          // Library for using SPI Communication 
#include <mcp2515.h>      // Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)

const int ARRAY_SIZE = 5;
const int number_of_fins = 40;
unsigned long delta_array[ARRAY_SIZE];
int currentIndex = 0;
const int can_pin = 10;   // Assuming CAN pin is 10 like in your working code
const int sensorinput = 5;

unsigned long delta;
unsigned long average_rpm;

struct can_frame canMsg;
MCP2515 mcp2515(can_pin);

void setup() {
  Serial.begin(9600);
  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  pinMode(sensorinput, INPUT);
  delay(10);
}

// Gets delta
unsigned long get_delta() {
  return pulseIn(sensorinput, HIGH);
}

// Calculates the average delta of fin detections in the array
unsigned long calc_average_delta() {
  unsigned long sum_of_deltas = 0;
  // Calculate the sum of time differences
  for (int i = 0; i < ARRAY_SIZE; i++) {
    sum_of_deltas += delta_array[i];
  }
  // Calculate average time difference
  return sum_of_deltas / ARRAY_SIZE;
}

// Func to work out the average rpm based on the time deltas in the array
unsigned long calc_average_rpm() {
  unsigned long average_delta = calc_average_delta();
  // Compute RPM
  if (average_delta != 0) {
    average_rpm = (60 * 1000000.0) / (average_delta * number_of_fins * 2);
  } else {
    average_rpm = 0;  // Avoid division by zero
  }

  Serial.print("rpm: ");
  Serial.println(average_rpm);
}

void add_value(){
  // Shift all values to the right
  for (int i = ARRAY_SIZE - 1; i > 0; i--) {
    delta_array[i] = delta_array[i - 1];
  }
  // Insert the new value at the beginning
  delta_array[0] = delta;
}

void zero_checker(){
  if ((delta_array[0] == 0) && (delta_array[1] == 0) && (delta_array[2] == 0)){
      for (int i = 0; i > ARRAY_SIZE - 1; i++) {
        delta_array[i] = 0;
      }
  }
}

void big_checker(){
  if ((delta_array[0] > 500000) && (delta_array[1] > 500000) && (delta_array[2] > 500000)){
      for (int i = 0; i > ARRAY_SIZE - 1; i++) {
        delta_array[i] = 0;
      }
  }
}

void sendRPM() {
  uint16_t rpm1 = average_rpm;

  canMsg.can_id  = 0x525;           // CAN id as 0x525
  canMsg.can_dlc = 8;               // CAN data length as 8
  canMsg.data[0] = (rpm1 & 0xFF);   // Send the lower byte
  canMsg.data[1] = ((rpm1 >> 8) & 0xFF); // Send the upper byte
  canMsg.data[2] = (rpm1 & 0xFF);   // Send the lower byte
  canMsg.data[3] = ((rpm1 >> 8) & 0xFF); // Send the upper byte
  canMsg.data[4] = (rpm1 & 0xFF);   // Send the lower byte
  canMsg.data[5] = ((rpm1 >> 8) & 0xFF); // Send the upper byte
  canMsg.data[6] = (rpm1 & 0xFF);   // Send the lower byte
  canMsg.data[7] = ((rpm1 >> 8) & 0xFF); // Send the upper byte
 
  mcp2515.sendMessage(&canMsg);     // Sends the CAN message
  Serial.println("sent");
}



void loop() {
  delta = get_delta();
  add_value();
  zero_checker();
  big_checker();
  calc_average_rpm();
  sendRPM();
  delay(1); // Add delay to avoid flooding the CAN bus
}
