#include <Arduino.h>

const int ARRAY_SIZE = 20;
const int number_of_fins = 40;
unsigned long delta_array[ARRAY_SIZE];
int currentIndex = 0;

const int sensorinput = 10;

unsigned long delta;
unsigned long rpm

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


void send_can(){


}


//main program
void run_average_rpm(){

  get_delta();
  delta_array[currentIndex] = delta;

  if (currentIndex >= ARRAY_SIZE) {
    array_average_rpm();
    currentIndex = 0;

    send_can();

  } else{
    currentIndex++;
  }
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  run_average_rpm();
}
