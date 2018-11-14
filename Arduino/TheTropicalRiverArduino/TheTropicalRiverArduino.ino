#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_MAX31856.h>

Adafruit_MCP4725 dac;
Adafruit_MAX31856 max = Adafruit_MAX31856(10, 11, 12, 13);
String id = "river";
bool isTemperatureSensor = false;


void setup(void) {
  Serial.begin(115200);
  Serial.setTimeout(10);
  dac.begin(0x62);
  max.begin();
}


void loop(void){  

  if (isTemperatureSensor) {
    String msg = "temperature:" + String(max.readThermocoupleTemperature());
    Serial.println(msg);
    delay(1000);
  }
  
  if (Serial.available()) {
    String input = Serial.readString();
    input.trim();

    // Parsing input: command:value
    int delimiterIndex = input.indexOf(':');
    String command;
    String value;
    if (delimiterIndex != -1) {
      command = input.substring(0, delimiterIndex);
      value = input.substring(delimiterIndex + 1, input.length());
    }

  if (input.equals("getId")) {
      Serial.println(id);
    }
   else if (command == "setSpeed") {
      double val = value.toDouble()*27.3;
      dac.setVoltage(val, false);
   }
   else if (command == "stop") {
      dac.setVoltage(0, false);
   }
   else if (input == "startTemperature") {
      isTemperatureSensor = true;
   }
   else if (input == "stopTemperature") {
      isTemperatureSensor = false;
   }
  }
}
