void setup() {                

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  // Serial communication  
  Serial.begin(115200);
  Serial.setTimeout(10);

}

void loop() {
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
      Serial.println("valves");
    }
    else if (input.equals("set")) {
      if (digitalRead(2) == HIGH) {
        Serial.println("ON");
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
      }
      else if (digitalRead(2) == LOW) {
        Serial.println("OFF");
        digitalWrite(2, HIGH);
        digitalWrite(3, LOW);
      }
    }
      
    }

}
