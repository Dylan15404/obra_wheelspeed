#include <Arduino.h>
#include <mcp2515.h>      //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)

const int ARRAY_SIZE = 20;
const int number_of_fins = 40;
unsigned long delta_array[ARRAY_SIZE];
int currentIndex = 0;
const int can_pin = 8;

const int sensorinput = 5;

unsigned long delta;
unsigned long rpm;

struct can_frame canMsg;
MCP2515 mcp2515(can_pin);

// gets delta
int get_delta() {
  delta = pulseIn(sensorinput, HIGH);
}


//calculates the average delta of fin detections in the array
unsigned long array_average_delta(){
  unsigned long sum_of_deltas = 0;
  // Calculate the sum of time differences
  for (int i = 0; i < ARRAY_SIZE; i++) {
    sum_of_deltas += delta_array[i];
  }
  //calculate average time difference
  unsigned long average_delta = sum_of_deltas / (ARRAY_SIZE);
  return average_delta;
}


//func to work out the average rpm based on the time deltas in the array
long array_average_rpm(){
  long average_delta = array_average_delta();
  
  // Compute RPM
  rpm = (60 * 1000000.0) / (average_delta * number_of_fins * 2);

  //Serial.print("rpm: ");
  Serial.println(rpm);
}


//main program
void run_average_rpm(){

  get_delta();
  delta_array[currentIndex] = delta;

  if (currentIndex >= ARRAY_SIZE) {
    array_average_rpm();
    currentIndex = 0;


  } else{
    currentIndex++;
  }
}

void sendRPM() {
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
  
}
