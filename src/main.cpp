#include <Arduino.h>
#include "SDCardModule/SDCard.h"
#include <SensirionI2CScd4x.h>
#include <Wire.h>


SDCard sd;
SensirionI2CScd4x sensor;
uint16_t co2_ppm = 0;
float temperature=0, humidity = 0;
unsigned long timestampLED = 0;
void updateLEDsTimed(uint16_t value);


//===================================================================== Settings
const int LEDPin_green = 14;
const int LEDPin_yellow = 12;
const int LEDPin_red = 13;
#define SDA 33
#define SCL 32

uint16_t limit1 = 1000;
uint16_t limit2 = 1400;

int updateLEDInterval_sec = 1; // min. 5 seconds (sensor update interval)
int logValueInterval_sec = 60; // > updateLEDInterval




void setup() {
  Serial.begin(115200);
  pinMode(LEDPin_green, OUTPUT);
  pinMode(LEDPin_yellow, OUTPUT);
  pinMode(LEDPin_red, OUTPUT);
  Wire.begin(SDA, SCL);
  int initValidation = 0;
  delay(1000);
  sensor.begin(Wire);
  delay(1000);
  initValidation += sd.init(logValueInterval_sec);
  initValidation += sensor.stopPeriodicMeasurement();
  initValidation += sensor.startPeriodicMeasurement();

  if(initValidation != 0)
  {
    Serial.println("Initialization failed - Check wiring and restart system.");
    Serial.println("Error Code: "+(String)initValidation);
    while(true);

  }
  else{
    Serial.println("Initialization successful");
  }


}

void loop() {
  sensor.readMeasurement(co2_ppm, temperature, humidity);
  updateLEDsTimed(co2_ppm);
  Serial.println((String)co2_ppm+";"+(String)temperature+";"+(String)humidity);
  sd.logDataTimed((String)co2_ppm+";"+(String)temperature+";"+(String)humidity);
  delay(500);
}



void updateLEDsTimed(uint16_t value)
{
  if(millis()-timestampLED>updateLEDInterval_sec*1000)
  {
    if(value < limit1)
    {
      digitalWrite(LEDPin_green, HIGH);
      digitalWrite(LEDPin_yellow, LOW);
      digitalWrite(LEDPin_red, LOW);
    }
    else if(value > limit1 && value < limit2)
    {
      digitalWrite(LEDPin_green, LOW);
      digitalWrite(LEDPin_yellow, HIGH);
      digitalWrite(LEDPin_red, LOW);
    }
    else{
      digitalWrite(LEDPin_green, LOW);
      digitalWrite(LEDPin_yellow, LOW);
      digitalWrite(LEDPin_red, HIGH);
    }
    timestampLED = millis();
  }
  
}