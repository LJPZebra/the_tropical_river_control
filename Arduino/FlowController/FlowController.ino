/* ----------------------------------------------------------------- *\
  _____ _                ____            _             _ _
  |  ___| | _____      __/ ___|___  _ __ | |_ _ __ ___ | | | ___ _ __
  | |_  | |/ _ \ \ /\ / / |   / _ \| '_ \| __| '__/ _ \| | |/ _ \ '__|
  |  _| | | (_) \ V  V /| |__| (_) | | | | |_| | | (_) | | |  __/ |
  |_|   |_|\___/ \_/\_/  \____\___/|_| |_|\__|_|  \___/|_|_|\___|_|

  To control a customized Aalborg SMV-010007 valve with a touchscreen.

  Téléverser avec le proMini

  By Raphaël Candelier (raphael.candelier@upmc.fr)

  \* ----------------------------------------------------------------- */

// === Includes ====================================================

#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "TouchScreen.h"
#include <EEPROM.h>

// === Definitions =================================================

String id = "fl1";

int pos = 0;
float flow;
float a, b;

// --- Display -----------------------------------------------------

String Mode = "Main";
String controlMode = "Pos";
String Input, prevStr, inputStr;

// --- TFT

// NB: The display also uses hardware SPI (pins 11, 12 and 13)

#define TFT_CS 10
#define TFT_DC 9

// --- Touch screen

#define YP A2
#define XM A3
#define YM 5
#define XP 4

// Calibration data for the touch screen
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define ILI9341_LIGHTSKYBLUE 0xB71D

// --- Motor -------------------------------------------------------

int stepTime = 100;

// --- Driver

const int pStep = 6;   // Step
const int pDir = 7;    // Dir
const int pEnable = 8; // Enable

// --- Switches

const int pSw0 = 2;   // Totally Closed switch
const int pSw1 = 3;   // Totally opened switch

// === Initialization ==============================================

// Display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Touchscreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// === Setup =======================================================

void setup() {

  // --- Serial communication

  Serial.begin(115200);
  Serial.setTimeout(5);

  // --- Get parameters from EEPROM

  // Get a
  EEPROM.get(0, a);
  if (isnan(a)) {
    EEPROM.put(0, 0.0f);
    EEPROM.get(0, a);
  }

  // Get b
  EEPROM.get(sizeof(float), b);
  if (isnan(b)) {
    EEPROM.put(sizeof(float), 0.0f);
    EEPROM.get(sizeof(float), b);
  }

  flow = pos2flow(pos);

  // --- Display

  tft.begin();
  tft.setRotation(2);

  // --- Pin setup

  pinMode(pSw0, INPUT_PULLUP);
  pinMode(pSw1, INPUT_PULLUP);

  pinMode(pStep, OUTPUT);
  digitalWrite(pStep, LOW);

  pinMode(pDir, OUTPUT);
  digitalWrite(pDir, HIGH);          // LOW to open ; HIGH to close
  
  pinMode(pEnable, OUTPUT);
  digitalWrite(pEnable, HIGH);

  /// Initialization
  
  resetMotor();
  updateDisplay();
  
}

// === Main loop ===================================================

void loop() {

  // --- Manage inputs ---------------------------------------------

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

    // --- Get information
    if (input.equals("getId")) {
      Serial.println(String(id));
    }
    else if (input.equals("listCommands")){
      Serial.println("stop dispPosition dispFlow setPosition setFlow setA setB");
    } else if (input.equals("stop")) {
      resetMotor();
      updateDisplay();
      Serial.println("stop");

    // --- Display position
    } else if (input.equals("dispPosition")) {

      controlMode = "Pos"; 
      drawFlowDisplay();

    // --- Display flow
    } else if (input.equals("dispFlow")) {

      controlMode = "Flow"; 
      drawFlowDisplay();

    // --- Set position
    } else if (command.equals("setPosition")) {
      moveTo(value.toInt());
      Serial.println(command + ":"  + value);

    // --- Set flow
    } else if (command.equals("setFlow")) {

      moveTo(flow2pos(input.substring(2).toFloat()));
      Serial.println(command + ":"  + value);

    // --- Set a
    } else if (command.equals("setA")) {

       a = value.toFloat();
       EEPROM.put(0, a);
       flow = pos2flow(pos);
       updateDisplay();
       Serial.println(command + ":"  + value);

    // --- Set b
    } else if (command.equals("setB")) {

       b = value.toFloat();
       EEPROM.put(sizeof(float), b);
       flow = pos2flow(pos);
       updateDisplay();
       Serial.println(command + ":"  + value);
  
    }
    
  }

  // --- Actions ---------------------------------------------------

  // Retrieve a point
  TSPoint p = ts.getPoint();

  if (p.z >= MINPRESSURE && p.z <= MAXPRESSURE) {

    // Get pixel coordinates
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    // Get corresponding character
    char c = getInput(p);

    // Process

    if (Mode == "Main") {

      inputStr = "";

      if (c == 'S') { moveTo(0); }
      if (c == 'P') { controlMode = "Pos"; drawFlowDisplay(); }
      if (c == 'F') { controlMode = "Flow"; drawFlowDisplay(); }

      if (c == 'a') {
        Input = "a";
        prevStr = String(a);
        Mode = "Keyboard";
        updateDisplay();
        return;
      }

      if (c == 'b') {
        Input = "b";
        prevStr = String(b);
        Mode = "Keyboard";
        updateDisplay();
        return;
      }

      if (c == 'v') {

        if (controlMode == "Pos") {
          Input = "P";
          prevStr = String(pos);
        }
        if (controlMode == "Flow") {
          Input = "F";
          prevStr = String(flow);
        }

        Mode = "Keyboard";
        updateDisplay();
        return;

      }

    }

    if (Mode == "Keyboard") {

      if (c == 'C') {
        inputStr = "";
        drawValue();
      }
      else if (c == 'D') {
        inputStr.remove(inputStr.length() - 1);
        drawValue();
      }
      else if (c == 'B') {
        Mode = "Main";
        updateDisplay();
      }
      else if (c == 'V') {

        if (Input == "P") {
          moveTo(inputStr.toInt());
        }
        if (Input == "F") {
          moveTo(flow2pos(inputStr.toFloat()));
        }
        if (Input == "a") {
          a = inputStr.toFloat();
          EEPROM.put(0, a);
          flow = pos2flow(pos);
        }
        if (Input == "b") {
          b = inputStr.toFloat();
          EEPROM.put(sizeof(float), b);
          flow = pos2flow(pos);
        }

        Mode = "Main";
        updateDisplay();

      } else {
        inputStr += c;
        drawValue();
      }

    }

    delay(200);

  }

}

void resetMotor() {

  // Close valve
  digitalWrite(pEnable, LOW);
  digitalWrite(pDir, HIGH);

  while (digitalRead(pSw0)==HIGH) {
    
    digitalWrite(pStep, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(pStep, LOW);
    delayMicroseconds(stepTime);
    
  }

  digitalWrite(pEnable, HIGH);

  // Update
  pos = 0;
  flow = 0;
  
}

void moveTo(int tpos) {

  // --- Movement

  digitalWrite(pEnable, LOW);

  // Set direction
  if (tpos>pos) { digitalWrite(pDir, LOW); }
  else { digitalWrite(pDir, HIGH); }

  // Make steps
  for (int i=0 ; i<abs(tpos-pos) ; i++) {
    digitalWrite(pStep, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(pStep, LOW);
    delayMicroseconds(stepTime);
  }

  digitalWrite(pEnable, HIGH);

  // --- Update
  
  pos = tpos;
  flow = pos2flow(pos);

  if (Mode == "Main") {
    drawFlowDisplay();
  }

}

float pos2flow(int p) {
  if (p==0) { return 0.0f; }
  return max(0.0f, a * p + b);
}

int flow2pos(float f) {
  if (f <= 0) { return 0; }
  return max(0, round((f - b) / a));
}

void updateDisplay() {

  if (Mode == "Main") {
    drawMain();
  }
  if (Mode == "Keyboard") {
    drawKeyboard();
  }

}

void drawMain(void) {

  tft.fillScreen(ILI9341_BLACK);

  // --- ID
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.setCursor(190, 5);
  tft.print("ID: " + id);

  // --- Title
  tft.setTextColor(ILI9341_ORANGE);
  tft.setTextSize(3);
  tft.setCursor(30, 5);
  tft.print("Flow");
  tft.setCursor(30, 40);
  tft.print("Controller");

  // --- Flow

  drawFlowDisplay();

  // --- Menu

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.setCursor(5, 158);
  tft.print("F = a.Pos + b ; a=" + String(a) + " ; b=" + String(b));

  tft.fillRoundRect(8, 175, 50, 40, 5, ILI9341_DARKGREY);
  tft.setTextColor(ILI9341_LIGHTGREY);
  tft.setTextSize(2);
  tft.setCursor(15, 187);
  tft.print("Pos");

  tft.fillRoundRect(66, 175, 50, 40, 5, ILI9341_DARKGREY);
  tft.setCursor(86, 187);
  tft.print("F");

  tft.fillRoundRect(124, 175, 50, 40, 5, ILI9341_LIGHTGREY);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(144, 187);
  tft.print("a");

  tft.fillRoundRect(182, 175, 50, 40, 5, ILI9341_LIGHTGREY);
  tft.setCursor(202, 187);
  tft.print("b");


  // --- Stop

  tft.fillRoundRect(5, 230, 230, 85, 10, ILI9341_RED);

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(90, 263);
  tft.print("STOP");

}

void drawFlowDisplay() {

  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(3);

  if (flow > 0) {

    tft.fillRect(0, 80, 240, 60, ILI9341_BLACK);
    tft.fillRoundRect(40, 80, 190, 60, 5, ILI9341_LIGHTSKYBLUE);
    tft.fillRect(35, 80, 10, 60, ILI9341_LIGHTSKYBLUE);
    tft.fillTriangle(5, 110, 35, 80, 35, 139, ILI9341_LIGHTSKYBLUE);

    tft.setTextSize(2);

    String tmp;
    if (controlMode == "Pos") {
      tmp = "Pos " + String(pos);
    }
    if (controlMode == "Flow") {
      tmp = String(flow) + " mL/s";
    }

    tft.setCursor(120 - tmp.length() * 6, 102);
    tft.print(tmp);

  } else {

    tft.fillRoundRect(5, 80, 230, 60, 5, ILI9341_LIGHTGREY);

    tft.setTextSize(3);
    tft.setCursor(60, 100);
    tft.print("STOPPED");

  }
}

void drawKeyboard(void) {

  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_LIGHTGREY);
  tft.setTextSize(2);
  tft.setCursor(240 - prevStr.length() * 12, 0);
  tft.print(prevStr);

  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print(Input + " = ");

  drawValue();

  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);

  // Point
  tft.fillRoundRect(8, 262, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(28, 280);
  tft.print(".");

  // Zero
  tft.fillRoundRect(66, 262, 108, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(118, 280);
  tft.print("0");

  // One
  tft.fillRoundRect(8, 204, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(28, 222);
  tft.print("1");

  // Two
  tft.fillRoundRect(66, 204, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(86, 222);
  tft.print("2");

  // Three
  tft.fillRoundRect(124, 204, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(144, 222);
  tft.print("3");

  // Four
  tft.fillRoundRect(8, 146, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(28, 164);
  tft.print("4");

  // Five
  tft.fillRoundRect(66, 146, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(86, 164);
  tft.print("5");

  // Six
  tft.fillRoundRect(124, 146, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(144, 164);
  tft.print("6");

  // Seven
  tft.fillRoundRect(8, 88, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(28, 106);
  tft.print("7");

  // Height
  tft.fillRoundRect(66, 88, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(86, 106);
  tft.print("8");

  // Nine
  tft.fillRoundRect(124, 88, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(144, 106);
  tft.print("9");

  // Back
  tft.fillRoundRect(182, 88, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(190, 106);
  tft.print("BCK");

  // Clear
  tft.fillRoundRect(182, 146, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(190, 164);
  tft.print("CLR");

  // Del
  tft.fillRoundRect(182, 204, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(190, 222);
  tft.print("DEL");


  // Enter
  tft.fillRoundRect(182, 262, 50, 50, 5, ILI9341_LIGHTGREY);
  tft.setCursor(190, 280);
  tft.print("VAL");

}

void drawValue() {

  tft.fillRect(0, 25, 240, 60, ILI9341_BLACK);

  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(4);

  tft.setCursor(235 - inputStr.length() * 25, 25);
  tft.print(inputStr);

}

char getInput(TSPoint p) {

  /*
  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("\tPressure = "); Serial.println(p.z);
  /**/

  if (Mode == "Main") {

    if (p.y <= 85) {
      return 'S';  // STOP
    }

    if (p.y > 85 && p.y <= 145) {
      if (p.x <= 62) {
        return 'b';  // b
      }
      if (p.x > 62 && p.x <= 120) {
        return 'a';  // a
      }
      if (p.x > 120 && p.x <= 178) {
        return 'F';  // Flow
      }
      if (p.x > 178) {
        return 'P';  // Position
      }
    }

    if (p.y >= 170 && p.y <= 230) {
      return ('v');  // Value
    }

  }

  if (Mode == "Keyboard") {

    if (p.x <= 54) {
      if (p.y >= 0 && p.y <= 54) {
        return 'V';  // VAL
      }
      if (p.y > 54 && p.y <= 108) {
        return 'D';  // DEL
      }
      if (p.y > 108 && p.y <= 162) {
        return 'C';  // CLR
      }
      if (p.y > 162 && p.y <= 216) {
        return 'B';  // BCK
      }
    }

    if (p.x > 54 && p.x <= 108) {
      if (p.y >= 0 && p.y <= 54) {
        return '0';  // 0
      }
      if (p.y > 54 && p.y <= 108) {
        return '3';  // 3
      }
      if (p.y > 108 && p.y <= 162) {
        return '6';  // 6
      }
      if (p.y > 162 && p.y <= 216) {
        return '9';  // 9
      }
    }

    if (p.x > 108 && p.x <= 162) {
      if (p.y >= 0 && p.y <= 54) {
        return '0';  // 0
      }
      if (p.y > 54 && p.y <= 108) {
        return '2';  // 2
      }
      if (p.y > 108 && p.y <= 162) {
        return '5';  // 5
      }
      if (p.y > 162 && p.y <= 216) {
        return '8';  // 8
      }
    }

    if (p.x > 162) {
      if (p.y >= 0 && p.y <= 54) {
        return '.';  // .
      }
      if (p.y > 54 && p.y <= 108) {
        return '1';  // 1
      }
      if (p.y > 108 && p.y <= 162) {
        return '4';  // 4
      }
      if (p.y > 162 && p.y <= 216) {
        return '7';  // 7
      }
    }

  }

  return '-';
}
