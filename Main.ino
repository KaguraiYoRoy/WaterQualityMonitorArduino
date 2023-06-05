#include "TDS.h"
#include "LM35.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "PH.h"
#include "Turbidity.h"

#define ATdsSensorPin       A0
#define ALM35SensorPin      A1
#define APHSensorPin        A2
#define ATurbiditySensorPin A3
#define DDS18B20SensorPin   2

OneWire oneWire(DDS18B20SensorPin);
DallasTemperature sensors(&oneWire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  pinMode(ATdsSensorPin, INPUT);
  pinMode(ALM35SensorPin, INPUT);
  pinMode(APHSensorPin, INPUT);
  pinMode(ATurbiditySensorPin, INPUT);
  pinMode(DDS18B20SensorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float WaterTempValue;

  sensors.requestTemperatures();
  WaterTempValue = sensors.getTempCByIndex(0);
  
  Serial.print("WaterTemp: ");
  Serial.print(WaterTempValue);
  Serial.print(" TDS:");
  Serial.print(GetTdsValue(ATdsSensorPin),0);
  Serial.print(" LM35:");
  Serial.print(GetLM35Value(ALM35SensorPin));
  Serial.print(" PH:");
  Serial.print(GetPHvalue(APHSensorPin, WaterTempValue));
  Serial.print(" Turbidity:");
  Serial.print(GetTurbidityValue(ATurbiditySensorPin));
  Serial.println(".");
}
