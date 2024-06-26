#include <Arduino.h>
<<<<<<< HEAD:old/obra_minimal_average.ino
#include <mcp2515.h>      //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
=======
#include <SPI.h>          // Library for using SPI Communication 
#include <mcp2515.h>      // Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
>>>>>>> 1cba398fe8a8d0cb6984513df0fea1a400dcb5f7:obra_minimal_average.ino

const int ARRAY_SIZE = 5;
const int number_of_fins = 40;
unsigned long delta_array[ARRAY_SIZE];
int currentIndex = 0;
<<<<<<< HEAD:old/obra_minimal_average.ino
const int can_pin = 8;

=======
const int can_pin = 10;   // Assuming CAN pin is 10 like in your working code
>>>>>>> 1cba398fe8a8d0cb6984513df0fea1a400dcb5f7:obra_minimal_average.ino
const int sensorinput = 5;

unsigned long delta;
unsigned long rpm;
<<<<<<< HEAD:old/obra_minimal_average.ino

struct can_frame canMsg;
MCP2515 mcp2515(can_pin);
=======
>>>>>>> 1cba398fe8a8d0cb6984513df0fea1a400dcb5f7:obra_minimal_average.ino

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
unsigned long array_average_delta() {
  unsigned long sum_of_deltas = 0;
  // Calculate the sum of time differences
  for (int i = 0; i < ARRAY_SIZE; i++) {
    sum_of_deltas += delta_array[i];
  }
  // Calculate average time difference
  return sum_of_deltas / ARRAY_SIZE;
}

// Func to work out the average rpm based on the time deltas in the array
unsigned long array_average_rpm() {
  unsigned long average_delta = array_average_delta();
  // Compute RPM
  if (average_delta != 0) {
    rpm = (60 * 1000000.0) / (average_delta * number_of_fins * 2);
  } else {
    rpm = 0;  // Avoid division by zero
  }

  Serial.print("rpm: ");
  Serial.println(rpm);

  return rpm;
}

<<<<<<< HEAD:old/obra_minimal_average.ino

//main program
void run_average_rpm(){

  get_delta();
=======
// Main program
void run_average_rpm() {
  delta = get_delta();
>>>>>>> 1cba398fe8a8d0cb6984513df0fea1a400dcb5f7:obra_minimal_average.ino
  delta_array[currentIndex] = delta;

  if (currentIndex >= ARRAY_SIZE - 1) {
    array_average_rpm();
    currentIndex = 0;
<<<<<<< HEAD:old/obra_minimal_average.ino


  } else{
=======
  } else {
>>>>>>> 1cba398fe8a8d0cb6984513df0fea1a400dcb5f7:obra_minimal_average.ino
    currentIndex++;
  }
}

void sendRPM() {
<<<<<<< HEAD:old/obra_minimal_average.ino
  run_average_rpm();
  uint16_t rpm1,rpm2,rpm3,rpm4 = rpm;
  canMsg.can_id  = 0x525;           //CAN id as 0x525
  canMsg.can_dlc = 8;               //CAN data length as 8
  canMsg.data[0] = (rpm1 & 0xFF); // Send the upper bytefirst             
  canMsg.data[1] = ((rpm1 >> 8) & 0xFF); // Send the lower byte
  canMsg.data[2] = (rpm2 & 0xFF); // Send the upper bytefirst 
  canMsg.data[3] = ((rpm2 >> 8) & 0xFF); // Send the lower byte
  canMsg.data[4] = (rpm3 & 0xFF); // Send the upper bytefirst 
  canMsg.data[5] = ((rpm3 >> 8) & 0xFF); // Send the lower byte
  canMsg.data[6] = (rpm4 & 0xFF); // Send the upper bytefirst 
  canMsg.data[7] = ((rpm4 >> 8) & 0xFF); // Send the lower byte
 
  mcp2515.sendMessage(&canMsg);     //Sends the CAN message
  Serial.println(rpm1);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();

}

void loop() {
  sendRPM();
  
=======
  uint16_t rpm1 = rpm;

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
  run_average_rpm();
  sendRPM();
  delay(1); // Add delay to avoid flooding the CAN bus
>>>>>>> 1cba398fe8a8d0cb6984513df0fea1a400dcb5f7:obra_minimal_average.ino
}
