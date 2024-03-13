#include <Arduino.h>

const int ARRAY_SIZE = 100;
const int number_of_fins = 42;
signed long delta_array[ARRAY_SIZE]; // Adjust array size according to your needs
int counter = 0;
unsigned long last_time_reading = micros();
int last_delta;
bool inputer = true;

//func to check if a fin is currently being seen
//obviously needs to be changed to actually implementation
int check_input() {
  unsigned long current;
  if (inputer) {
    current = micros(); // Arduino equivalent of time.time()
    last_delta = current - last_time_reading;
    last_time_reading = current;
  }
}

//send can message func
//obviously needs to be changed to actually implementation
void send_can() {
  if (last_delta != 0){
    Serial.println(last_delta);
  }
}

//calculates the average delta of fin detections in the array
int array_average_delta(){
  int sum_of_deltas = 0;
  // Calculate the sum of time differences
  for (int i = 1; i < ARRAY_SIZE; i++) {
    sum_of_deltas += delta_array[i];
  }
  
  //calculate average time difference
  unsigned long average_delta = sum_of_deltas / (ARRAY_SIZE - 1);
  return average_delta;
}

//func to calculate the average sample rate using the time deltas in the array
float calculate_sample_rate(){
  float average_delta = array_average_delta();
  
  // Compute the sample rate
  float sample_rate =  1000000.0 / average_delta;
  Serial.print("sample rate: ");
  Serial.println(sample_rate);
  return sample_rate;
}

//func to work out the average rpm based on the time deltas in the array
float array_average_rpm(){
  int average_delta = array_average_delta();
  
  // Compute RPM
  float rpm = (60.0 * 1000000.0) / (average_delta * number_of_fins);
  Serial.print("rpm: ");
  Serial.println(rpm);
  return rpm;
}

//func to find the best time_delay within a range for the desired sample_rate
void tune_delay(int delay_range_start, int delay_range_end, int desired_sample_rate){
  int dict_keys_sample_rate[delay_range_end - delay_range_start];
  int dict_values_delays[delay_range_end - delay_range_start];
  int dict_counter = 0;
  for (int i = delay_range_start; i < delay_range_end; i++){
    for (int x = 0; x < 100; x++){
      delay(i);
      check_input();
      delta_array[counter] = last_delta;
      counter++;
      if (counter == 100) {
        send_can();
        counter = 0;
        }
        break;
      }
    unsigned long sr = calculate_sample_rate();

    Serial.print("sample rate: ");
    Serial.print(sr);
    Serial.print(" delay: ");
    Serial.println(i); 
       
    dict_keys_sample_rate[counter] = sr;
    dict_values_delays[counter] = i;
    counter++;
  }

  int min_difference;
  int closest_value;
  int closest_value_index;

  for (int i = delay_range_start + 1; i < delay_range_end; i++) {
      int difference = abs(desired_sample_rate - dict_keys_sample_rate[i]);
      if (difference < min_difference){
          min_difference = difference;
          closest_value = dict_keys_sample_rate[i]; // Corrected the index from x to i
          closest_value_index = i;
      }
  }

  Serial.print("closest sample rate: ");
  Serial.print(closest_value);
  Serial.print(" delay: ");
  Serial.println(dict_values_delays[closest_value_index]);

}

//main program
void run_program(){
  //a delay buffer would go here to alter sample rate
  //delay()
  check_input();
  delta_array[counter] = last_delta;
  counter++;
  if (counter == 100) {
    send_can();
    array_average_rpm();
    calculate_sample_rate();
    counter = 0;
  }
}

void setup() {
  Serial.begin(9600);

}

void loop() {
  run_program();
}
