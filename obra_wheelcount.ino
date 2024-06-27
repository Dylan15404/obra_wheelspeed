#include <Arduino.h>
#include <SPI.h>          // Library for using SPI Communication 
#include <mcp2515.h>      // Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)

const int number_of_fins = 40;
const int can_pin = 10;   // Assuming CAN pin is 10 like in your working code
const int sensorinput = 5;

int count = 0;
int wheelcount = 0;


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
void check() {
  attachInterrupt(digitalPinToInterrupt(sensorinput), counter, FALLING);
}

void counter(){
  count++;
}



void sendCOUNT() {

  canMsg.can_id  = 0x525;           // CAN id as 0x525
  canMsg.can_dlc = 8;               // CAN data length as 8
  canMsg.data[0] = (wheelcount & 0xFF);   // Send the lower byte
  canMsg.data[1] = ((wheelcount >> 8) & 0xFF); // Send the upper byte
  canMsg.data[2] = (wheelcount & 0xFF);   // Send the lower byte
  canMsg.data[3] = ((wheelcount >> 8) & 0xFF); // Send the upper byte
  canMsg.data[4] = (wheelcount & 0xFF);   // Send the lower byte
  canMsg.data[5] = ((wheelcount >> 8) & 0xFF); // Send the upper byte
  canMsg.data[6] = (wheelcount & 0xFF);   // Send the lower byte
  canMsg.data[7] = ((wheelcount >> 8) & 0xFF); // Send the upper byte
 
  mcp2515.sendMessage(&canMsg);     // Sends the CAN message
  //Serial.println("sent");
}



void loop() {
  check();
  if (count >= number_of_fins){
    wheelcount++;
  }
  sendCOUNT();
  delay(1);
}
