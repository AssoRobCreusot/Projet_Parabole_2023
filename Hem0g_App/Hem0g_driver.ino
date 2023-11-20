#include "ADS1115_WE.h" 
#include "Wire.h"

#define PUMP_FRONT_DIRECTION 0
#define PUMP_BACK_DIRECTION 1

#define PUMP_ENA 3
#define PUMP_IN1 5
#define PUMP_IN2 6

#define PRESSURE_SENSOR_1 A0
#define PRESSURE_SENSOR_2 A1

#define FLOW_SENSOR ADS1115_COMP_0_3
#define FLOW_ADC_ADDR 0x4A
#define FLOW_ADC_GAIN 0
#define FLOW_ADC_READING_MODE 1
#define FLOW_ADC_DATARATE 4



ADS1115_WE adc = ADS1115_WE(FLOW_ADC_ADDR);
int init_speed = 255;

void setup() {
  // I2C link
  Wire.begin();
  // Serial link
  Serial.begin(9600);
  // Configuration of the pump driver
  configureDriverDirection(0, PUMP_ENA, PUMP_IN1, PUMP_IN2);
  // Configuration of the ADC 
  if(!adc.init()){
    Serial.println("ADS1115 not connected!");
  }
  /* Set the voltage range of the ADC to adjust the gain
   * Please note that you must not apply more than VDD + 0.3V to the input pins!
   * 
   * ADS1115_RANGE_6144  ->  +/- 6144 mV
   * ADS1115_RANGE_4096  ->  +/- 4096 mV
   * ADS1115_RANGE_2048  ->  +/- 2048 mV (default)
   * ADS1115_RANGE_1024  ->  +/- 1024 mV
   * ADS1115_RANGE_0512  ->  +/- 512 mV
   * ADS1115_RANGE_0256  ->  +/- 256 mV
   */
  adc.setVoltageRange_mV(ADS1115_RANGE_0256); 
} 
  

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil(';'); // Read the input until the first ';'
    int value1 = input.toInt(); // Convert the string to an integer for the first value
    input = Serial.readStringUntil(';'); // Read the last part of the input
    input.remove(input.length() - 1); // Remove the 's' character at the end
    int timeInSeconds = input.toInt(); // Convert the string to an integer for time in seconds
    int timestart = millis();
    int timeInMillis = timeInSeconds * 1000;
    int tmpTime = 0;
    if (timeInMillis > 1000 and value1 > 50){
      Serial.println("start");
      setDriverSpeed(value1, PUMP_ENA);
     while(tmpTime <= timeInMillis){
        float flow = read_flow(FLOW_SENSOR);
        float pressure_1 = read_pressure(PRESSURE_SENSOR_1);
        float pressure_2 = read_pressure(PRESSURE_SENSOR_2);
        Serial.println(String(value1)+";"+String(pressure_1)+";"+String(pressure_2)+";"+String(flow));
        tmpTime = int(millis() - timestart);
        delay(100);
      }
      
      setDriverSpeed(0, PUMP_ENA);
      Serial.println("finish");
    }else{
      Serial.println("Error");
    }
     
  }
}


void configureDriverDirection(int rotation, int pin_enA, int pin_in1, int pin_in2){
  // Serial.println("Configuring the pin");
  pinMode(pin_in1, OUTPUT);
  pinMode(pin_in2, OUTPUT);
  pinMode(pin_enA, OUTPUT);

  // Serial.println("Configuring the motor direction");
  if(rotation == PUMP_FRONT_DIRECTION) {
    // Configuration du L298N en "marche avant", pour le moteur connecté au pont A. Selon sa table de vérité, il faut que :
    digitalWrite(pin_in1, HIGH);                 // L'entrée IN1 doit être au niveau haut
    digitalWrite(pin_in2, LOW);                  // L'entrée IN2 doit être au niveau bas    
  }
  
  if(rotation == PUMP_BACK_DIRECTION) {
    // Configuration du L298N en "marche arrière", pour le moteur câblé sur le pont A. Selon sa table de vérité, il faut que :
    digitalWrite(pin_in1, LOW);                  // L'entrée IN1 doit être au niveau bas
    digitalWrite(pin_in2, HIGH);                 // L'entrée IN2 doit être au niveau haut
  }
}

void setDriverSpeed(int speed, int pin_en){
  analogWrite(pin_en, speed);
}

int read_pressure(uint8_t pin){
  int max = 1024;
  int value = analogRead(pin);
  if (value <= max/10){
    value = max/10;
  }else if (value > (max/10)*9){
    value = (max/10)*9;
  }
  value = map(value, max/10, (max/10)*9, 0, 1034);
  return value;
}

float read_flow(ADS1115_MUX channel){
  float mesure = readChannel(channel);
  return mesure;
}

float readChannel(ADS1115_MUX channel) {
  float voltage = 0.0;
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while(adc.isBusy()){}
  voltage = adc.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}