#include <Arduino.h>
#include <mcp2515.h>      //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
#include <SPI.h>          // Library for using SPI Communication 


const int number_of_fins = 40;
const int sensorinput = 5;      // connect sensor to d4 works

unsigned long delta;
unsigned long rpm;

struct can_frame canMsg;
const int can_pin = 10;
MCP2515 mcp2515(can_pin);

unsigned long time1;
unsigned long time2;


void setup() {
  while (!Serial);
  Serial.begin(9600);
  SPI.begin();

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  pinMode(sensorinput, INPUT);
  time1 = millis();
  delay(10);
}

void get_delta(){

  delta = pulseIn(sensorinput, HIGH);

}

void calc_rpm(){
  
  if (delta != 0){
  // Compute RPM
  rpm = (60 * 1000000.0) / (delta * number_of_fins * 2);

  //Serial.println(rpm);

  }
}

void sendRPM() {



  uint16_t rpm1 = rpm;
  uint16_t rpm2 = rpm1;
  uint16_t rpm3 = rpm1;
  uint16_t rpm4 = rpm1;

  //Serial.println(rpm1);

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
  Serial.println("sent");

}



void loop() {

  get_delta();
  calc_rpm();

  time2 = millis();
  if ((time2-time1)>1){
    sendRPM();
    time1 = millis();
  }

  delay(10);
}
