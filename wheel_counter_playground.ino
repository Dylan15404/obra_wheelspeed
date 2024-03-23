#include <Arduino.h>

const int ARRAY_SIZE = 20;
const int number_of_fins = 40;
unsigned long delta_array[ARRAY_SIZE]; // Adjust array size according to your needs
int currentIndex = 0;

const int sensorinput = 10;      // connect sensor to d4 works
const int sensoroutput = 8;

unsigned long delta;

unsigned long last_timer;
unsigned long timer;

int freq;
unsigned long temp;

//gets delta
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


//func to calculate the average sample rate using the time deltas in the array
long calculate_sample_rate(){
  long average_delta = array_average_delta();
  
  // Compute the sample rate
  long sample_rate =  1000000.0 / average_delta;

  if (sample_rate>100000){
    sample_rate = 0;
  }

  Serial.print("sample rate: ");
  Serial.println(sample_rate);

  return sample_rate;
}

//func to work out the average rpm based on the time deltas in the array
long array_average_rpm(){
  long average_delta = array_average_delta();
  
  // Compute RPM
  long rpm = (60 * 1000000.0) / (average_delta * number_of_fins * 2);

  //Serial.print("rpm: ");
  Serial.println(rpm);

  return rpm;
}

long raw_rpm(){
  
  // Compute RPM
  long rpm = (60 * 1000000.0) / (delta * number_of_fins * 2);

  //Serial.print("rpm: ");
  Serial.println(rpm);
  return rpm;
}


long update_frequency(){
  timer = micros();
  
  freq = 1000000 / (timer - last_timer);
  Serial.print(freq);
  Serial.println("hz");


  analogWrite(sensoroutput, freq);


  last_timer = timer;
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

void run_raw_rpm(){
  if (get_delta() != 0){
    raw_rpm();
  }
}


void setup() {
  Serial.begin(9600);
  last_timer = micros();
  pinMode(sensoroutput, OUTPUT);
}

void loop() {
}
