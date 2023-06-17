// ###########################################################################################################################################
// #
// # Code for the printables 3x timers with 7-segment displays project:
// # https://www.printables.com/de/model/507138-3-timers-with-7-segment-displays
// #
// # Code by https://github.com/AWSW-de 
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/3x-timers-with-7-segment-displays/blob/main/LICENSE
// #
// ###########################################################################################################################################

// Version: V1.0.0

// ###########################################################################################################################################
// # Includes:
// ###########################################################################################################################################
#include "Arduino.h"
#include "Keypad.h"
#include "LedControl.h"

// ###########################################################################################################################################
// Keyboard matix button settings:
// ###########################################################################################################################################
const byte ROWS = 4;  // 4 rows
const byte COLS = 4;  // 4 columns
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// ###########################################################################################################################################
// Hardware settings:
// ###########################################################################################################################################
LedControl lcA = LedControl(27, 14, 12, 1);  // DataIn pin, CLK pin, CS load pin, amount of MAX72XX displays = 1! --> Display A
LedControl lcB = LedControl(33, 25, 26, 1);  // DataIn pin, CLK pin, CS load pin, amount of MAX72XX displays = 1! --> Display B
LedControl lcC = LedControl(15, 13, 32, 1);  // DataIn pin, CLK pin, CS load pin, amount of MAX72XX displays = 1! --> Display C
// Connect the keyboard pins from right to left:
byte rowPins[ROWS] = { 16, 4, 0, 2 };    //connect to the row pinouts of the keypad
byte colPins[COLS] = { 19, 18, 5, 17 };  //connect to the column pinouts of the keypad

// ###########################################################################################################################################
// Init values:
// ###########################################################################################################################################
unsigned long previousMillis = 0;
const long interval = 1000;
int display_brightness = 6;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ###########################################################################################################################################
// Startup actions:
// ###########################################################################################################################################
void setup() {
  Serial.begin(115200);

  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call:
  lcA.shutdown(0, false);
  lcB.shutdown(0, false);
  lcC.shutdown(0, false);
  // Set the brightness of the display:
  lcA.setIntensity(0, display_brightness);
  lcB.setIntensity(0, display_brightness);
  lcC.setIntensity(0, display_brightness);
  // Clear the display:
  lcA.clearDisplay(0);
  lcB.clearDisplay(0);
  lcC.clearDisplay(0);

  // Startup animation:
  for (int i = 0; i < 10; i++) {
    lcA.setChar(0, i, i, true);
    lcB.setChar(0, i, i, true);
    lcC.setChar(0, i, i, true);
    delay(100);
  }

   // Clear the display:
  lcA.clearDisplay(0);
  lcB.clearDisplay(0);
  lcC.clearDisplay(0);
}

// ###########################################################################################################################################
// Init Strings to store digits to:
// ###########################################################################################################################################
String timeA = "";
String timeB = "";
String timeC = "";
String timeD = "";
String timeX = "";
int timeS = 0;  // Digit number from left to right

// ###########################################################################################################################################
// Init all digits on the display:
// ###########################################################################################################################################
String tHAl = "0";
String tHAr = "0";
String tMAl = "0";
String tMAr = "0";
String tSAl = "0";
String tSAr = "0";
String tHBl = "0";
String tHBr = "0";
String tMBl = "0";
String tMBr = "0";
String tSBl = "0";
String tSBr = "0";
String tHCl = "0";
String tHCr = "0";
String tMCl = "0";
String tMCr = "0";
String tSCl = "0";
String tSCr = "0";
String tHA;
String tMA;
String tSA;
String tHB;
String tMB;
String tSB;
String tHC;
String tMC;
String tSC;

// ###########################################################################################################################################
// Timer status values:
// ###########################################################################################################################################
bool UseTimerA = 0;
bool UseTimerB = 0;
bool UseTimerC = 0;

// ###########################################################################################################################################
// Loop actions during runtime:
// ###########################################################################################################################################
void loop() {

  // Print the pressed key:
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key ");
    Serial.print(key);
    Serial.println(" was pressed");
  }

  // Check for selected timer A,B or C:
  if (key == 'A') {
    lcA.setChar(0, 7, '1', true);
    timeX = "A";                     // Select timer
    timeS = 0;                       // Digit
    if (timeD == "D") timeA = "DA";  // Delete timer
  }

  if (key == 'B') {
    lcB.setChar(0, 7, '2', true);
    timeX = "B";                     // Select timer
    timeS = 0;                       // Digit
    if (timeD == "D") timeB = "DB";  // Delete timer
  }

  if (key == 'C') {
    lcC.setChar(0, 7, '3', true);
    timeX = "C";                     // Select timer
    timeS = 0;                       // Digit
    if (timeD == "D") timeC = "DC";  // Delete timer
  }

  // Check the numbers input:
  if ((key == '1') || (key == '2') || (key == '3') || (key == '4') || (key == '5') || (key == '6') || (key == '7') || (key == '8') || (key == '9') || (key == '0')) {
    numberinput(timeX, key);
  }

  // Delete timer:
  if (key == 'D') {
    if (timeD == "D") {
      timeD = "DD";
    } else {
      timeD = "D";
    }
  }

  // Delete timer:
  if (timeD == "DD") {  // Delete all timers at once
    timeA = "DA";
    timeB = "DB";
    timeC = "DC";
  }
  if (timeA == "DA") {  // Delete timer A
    lcA.clearDisplay(0);
    timeA = "";
    timeD = "";
    timeS = 0;
    tHAl = "0";
    tHAr = "0";
    tMAl = "0";
    tMAr = "0";
    tSAl = "0";
    tSAr = "0";
    tHA = "0";
    tMA = "0";
    tSA = "0";
    UseTimerA = 0;
  }

  if (timeB == "DB") {  // Delete timer B
    lcB.clearDisplay(0);
    timeB = "";
    timeD = "";
    timeS = 0;
    tHBl = "0";
    tHBr = "0";
    tMBl = "0";
    tMBr = "0";
    tSBl = "0";
    tSBr = "0";
    tHB = "0";
    tMB = "0";
    tSB = "0";
    UseTimerB = 0;
  }

  if (timeC == "DC") {  // Delete timer C
    lcC.clearDisplay(0);
    timeC = "";
    timeD = "";
    timeS = 0;
    tHCl = "0";
    tHCr = "0";
    tMCl = "0";
    tMCr = "0";
    tSCl = "0";
    tSCr = "0";
    tHC = "0";
    tMC = "0";
    tSC = "0";
    UseTimerC = 0;
  }

  // Update the displays:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Show Timer A:
    if (UseTimerA == 1) {
      if (tSA.toInt() > 0) {
        tSA = tSA.toInt() - 1;
      } else {
        if (tMA.toInt() > 0) {
          tMA = tMA.toInt() - 1;
          tSA = 59;
        } else {
          if (tHA.toInt() > 0) {
            tHA = tHA.toInt() - 1;
            tMA = 59;
            tSA = 59;
          } else {
            Serial.println("End of timer A");
            UseTimerA = 0;
            timeA = "DA";
          }
        }
      }
    }

    // Show Timer B:
    if (UseTimerB == 1) {
      if (tSB.toInt() > 0) {
        tSB = tSB.toInt() - 1;
      } else {
        if (tMB.toInt() > 0) {
          tMB = tMB.toInt() - 1;
          tSB = 59;
        } else {
          if (tHB.toInt() > 0) {
            tHB = tHB.toInt() - 1;
            tMB = 59;
            tSB = 59;
          } else {
            Serial.println("End of timer B");
            UseTimerB = 0;
            timeB = "DB";
          }
        }
      }
    }

    // Show Timer C:
    if (UseTimerC == 1) {
      if (tSC.toInt() > 0) {
        tSC = tSC.toInt() - 1;
      } else {
        if (tMC.toInt() > 0) {
          tMC = tMC.toInt() - 1;
          tSC = 59;
        } else {
          if (tHC.toInt() > 0) {
            tHC = tHC.toInt() - 1;
            tMC = 59;
            tSC = 59;
          } else {
            Serial.println("End of timer C");
            UseTimerC = 0;
            timeC = "DC";
          }
        }
      }
    }

    // Write on the display:
    writeArduinoOn7Segment();
  }
}

// ###########################################################################################################################################
// Get the number inputs:
// ###########################################################################################################################################
void numberinput(String timeX, char numinput) {
  // Serial.println(timeX);
  // Serial.println(numinput);
  if (timeX == "A") {
    switch (timeS) {
      case 0:
        {
          tHAl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 1:
        {
          tHAr = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 2:
        {
          tMAl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 3:
        {
          tMAr = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 4:
        {
          tSAl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 5:
        {
          tSAr = numinput - (int)48;
          timeS = 0;
          UseTimerA = 1;
          break;
        }
    }
    tHA = tHAl + tHAr;
    tMA = tMAl + tMAr;
    tSA = tSAl + tSAr;
  }

  if (timeX == "B") {
    switch (timeS) {
      case 0:
        {
          tHBl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 1:
        {
          tHBr = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 2:
        {
          tMBl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 3:
        {
          tMBr = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 4:
        {
          tSBl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 5:
        {
          tSBr = numinput - (int)48;
          timeS = 0;
          UseTimerB = 1;
          break;
        }
    }
    tHB = tHBl + tHBr;
    tMB = tMBl + tMBr;
    tSB = tSBl + tSBr;
  }

  if (timeX == "C") {
    switch (timeS) {
      case 0:
        {
          tHCl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 1:
        {
          tHCr = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 2:
        {
          tMCl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 3:
        {
          tMCr = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 4:
        {
          tSCl = numinput - (int)48;
          timeS = timeS + 1;
          break;
        }
      case 5:
        {
          tSCr = numinput - (int)48;
          timeS = 0;
          UseTimerC = 1;
          break;
        }
    }
    tHC = tHCl + tHCr;
    tMC = tMCl + tMCr;
    tSC = tSCl + tSCr;
  }
}

// ###########################################################################################################################################
// Write on the display:
// ###########################################################################################################################################
void writeArduinoOn7Segment() {
  lcA.setChar(0, 7, '1', true);
  lcA.setChar(0, 6, ' ', false);
  lcA.setChar(0, 5, tHA.toInt() / 10, false);
  lcA.setChar(0, 4, tHA.toInt() % 10, true);
  lcA.setChar(0, 3, tMA.toInt() / 10, false);
  lcA.setChar(0, 2, tMA.toInt() % 10, true);
  lcA.setChar(0, 1, tSA.toInt() / 10, false);
  lcA.setChar(0, 0, tSA.toInt() % 10, false);

  lcB.setChar(0, 7, '2', true);
  lcB.setChar(0, 6, ' ', false);
  lcB.setChar(0, 5, tHB.toInt() / 10, false);
  lcB.setChar(0, 4, tHB.toInt() % 10, true);
  lcB.setChar(0, 3, tMB.toInt() / 10, false);
  lcB.setChar(0, 2, tMB.toInt() % 10, true);
  lcB.setChar(0, 1, tSB.toInt() / 10, false);
  lcB.setChar(0, 0, tSB.toInt() % 10, false);

  lcC.setChar(0, 7, '3', true);
  lcC.setChar(0, 6, ' ', false);
  lcC.setChar(0, 5, tHC.toInt() / 10, false);
  lcC.setChar(0, 4, tHC.toInt() % 10, true);
  lcC.setChar(0, 3, tMC.toInt() / 10, false);
  lcC.setChar(0, 2, tMC.toInt() % 10, true);
  lcC.setChar(0, 1, tSC.toInt() / 10, false);
  lcC.setChar(0, 0, tSC.toInt() % 10, false);
}
