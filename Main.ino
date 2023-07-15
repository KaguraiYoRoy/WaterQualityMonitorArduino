#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "TDS.h"
#include "LM35.h"
#include "PH.h"
#include "Turbidity.h"

#define APHSensorPin        A0
#define ABatteryVinPin      A1
#define ALM35SensorPin      A2
#define ATurbiditySensorPin A3
#define ATdsSensorPin       A4
#define DDS18B20SensorPin   12

OneWire oneWire(DDS18B20SensorPin);
DallasTemperature sensors(&oneWire);

bool DebugMode;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  DebugMode = false;
  pinMode(ATdsSensorPin, INPUT);
  pinMode(ALM35SensorPin, INPUT);
  pinMode(APHSensorPin, INPUT);
  pinMode(ATurbiditySensorPin, INPUT);
  pinMode(ABatteryVinPin, INPUT);
  pinMode(DDS18B20SensorPin, INPUT);
}

void processcom(char cmd){
  StaticJsonDocument<200> jsonBuffer;
  String output;
  switch(cmd){
    case 'G':
    case 'g':{
      float WaterTempValue;

      sensors.requestTemperatures();
      WaterTempValue = sensors.getTempCByIndex(0);
  
      jsonBuffer["result"] = 0;
      jsonBuffer["msg"] = "OK";
      JsonObject Values = jsonBuffer.createNestedObject("Values");

      Values["WaterTemp"] = WaterTempValue;
      Values["TDS"] = GetTdsValue(ATdsSensorPin);
      Values["LM35"] = GetLM35Value(ALM35SensorPin);
      Values["PH"] = GetPHvalue(APHSensorPin, WaterTempValue);
      Values["Turbidity"] = GetTurbidityValue(ATurbiditySensorPin);
      Values["BatteryVoltage"] = analogRead(ABatteryVinPin) * (5.00 / 1023.00);
      
      if(Values["TDS"]<0)
        Values["TDS"]=0;
      
      serializeJson(jsonBuffer, output);
      if(DebugMode)
        Serial.println(output);
      else
        Serial.print(output);

      break;
    }
    case 'D':
    case 'd':{
      DebugMode = !DebugMode;
      jsonBuffer["result"]=0;
      jsonBuffer["msg"]="OK";
      jsonBuffer["Debug"]=DebugMode?"On":"Off";
      serializeJson(jsonBuffer, output);
      if(DebugMode)
        Serial.println(output);
      else
        Serial.print(output);
      break;
    }
    default:{
      jsonBuffer["result"]=-1;
      jsonBuffer["msg"]="Invalid Command";
      serializeJson(jsonBuffer, output);
      if(DebugMode)
        Serial.println(output);
      else
        Serial.print(output);
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
