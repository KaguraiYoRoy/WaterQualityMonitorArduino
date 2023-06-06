#include <ArduinoJson.h>

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

void processcom(char cmd){
  StaticJsonDocument<200> jsonBuffer;
  String output;
  switch(cmd){
    case 'g':{
      float WaterTempValue;

      sensors.requestTemperatures();
      WaterTempValue = sensors.getTempCByIndex(0);
  
      jsonBuffer["result "] = 0;
      jsonBuffer["msg"] = "OK";
      JsonObject Values = jsonBuffer.createNestedObject("Values");

      Values["WaterTemp"] = WaterTempValue;
      Values["TDS"] = GetTdsValue(ATdsSensorPin);
      Values["LM35"] = GetLM35Value(ALM35SensorPin);
      Values["PH"] = GetPHvalue(APHSensorPin, WaterTempValue);
      Values["Turbidity"] = GetTurbidityValue(ATurbiditySensorPin);
      
      serializeJson(jsonBuffer, output);
      Serial.println(output);

      break;
    }
    default:{
      jsonBuffer["result"]=-1;
      jsonBuffer["msg"]="Invalid Command";
      serializeJson(jsonBuffer, output);
      Serial.println(output);
      break;
    }
  }
    
}
void loop() {
  if(Serial.available()>0){
    if(Serial.peek()!='\n' && Serial.peek()!='\r'){
      processcom(Serial.read());
    }
    else Serial.read();
  }
  return;
}
