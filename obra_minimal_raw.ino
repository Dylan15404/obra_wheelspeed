#include <Arduino.h>

const int number_of_fins = 40;
const int sensorinput = 10;      // connect sensor to d4 works

unsigned long delta;
unsigned long rpm;

void setup() {

  Serial.begin(9600);
  
}

void get_delta(){

  delta = pulseIn(sensorinput, HIGH);

}

void calc_rpm(){
  
  if (delta != 0){
  // Compute RPM
  long rpm = (60 * 1000000.0) / (delta * number_of_fins * 2);

  //Serial.println(rpm);

}

void send_can(){

}



void loop() {

  check_input();
  calc_rpm();
  send_can();

}
