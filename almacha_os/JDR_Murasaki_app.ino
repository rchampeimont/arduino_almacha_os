#include <LiquidCrystal.h>

const int N_DICES = 15;
const int dices[N_DICES] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 20, 22, 24, 30, 100 };

// Murusaki digits logic is: the value is the number of angles
const byte murasakiDigits[5][8] = {
  // zero:
  {
    B00000,
    B00001,
    B00010,
    B00100,
    B01000,
    B10000,
    B00000,
  },
  // one:
  {
    B00000,
    B00001,
    B00011,
    B00101,
    B01001,
    B10001,
    B00000,
  },
  // two:
  {
    B00000,
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B00000,
  },
  // three:
  {
    B00000,
    B00001,
    B00011,
    B00101,
    B01001,
    B11111,
    B00000,
  },
  // four
  {
    B00000,
    B11111,
    B10001,
    B10001,
    B10001,
    B11111,
    B00000,
  },
}
;

class MurasakiApp {
  public:
    void setup() {
      // Create Murasaki numbers
      for (int i = 0; i < 5; i++) {
        lcd.createChar(i, murasakiDigits[i]);
      }

      lcd.clear();
      selectDice();
    }

    void loop() {
      unsigned long key = keyboardReadKey();

      if (key == 0xE074 || key == 0xE072) {
        // right or down arrow
        selectedDiceIndex++;
        if (selectedDiceIndex >= N_DICES) {
          selectedDiceIndex = 0;
        }
        selectDice();
      } else if (key == 0xE06B || key == 0xE075) {
        // left or up arrow
        if (selectedDiceIndex == 0) {
          selectedDiceIndex = N_DICES - 1;
        } else {
          selectedDiceIndex--;
        }
        selectDice();
      } else if (key == 0x5A) {
        rollDice();
      }
      updateMurasakiClock();
    }

  private:
    int selectedDiceIndex = 0;
    int selectedDiceType = 0;

    unsigned long secondsSinceStart = 0;

    void printMurasakiDigit(int value) {
      lcd.write(byte(value));
    }

    // Pass -1 as littleDigitCol if you want to just print "here" without setCursor
    void printMurasakiNumber(unsigned long value, int littleDigitCol, int row) {
      bool leadingZeros = true;
      unsigned long divider = 1220703125; // 5 to the 13
      for (int i = 13; i >= 0; i--) {
        int digit = value / divider;
        if (leadingZeros == true && (digit != 0 || i == 0)) {
          leadingZeros = false;
          if (littleDigitCol >= 0) {
            lcd.setCursor(littleDigitCol - i, row);
          }
        }
        if (! leadingZeros) {
          printMurasakiDigit(digit);
        }
        value -= digit * divider;
        divider = divider / 5;
      }
    }

    void printDiceType(int col, int row) {
      lcd.setCursor(col, row);
      // clear old displayed value
      lcd.print("    ");
      lcd.setCursor(col, row);
      lcd.print("D");
      lcd.print(selectedDiceType, DEC);
    }

    void selectDice() {
      Serial.print("Selected dice ");
      Serial.println(selectedDiceIndex);

      selectedDiceType = dices[selectedDiceIndex];
      printDiceType(0, 0);
    }


    void rollDice() {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("ROLL");

      randomSeed(millis());
      int value = random(1, selectedDiceType + 1);

      // animation
      lcd.setCursor(0, 1);
      for (int i = 0; i < LCD_COLS; i++) {
        lcd.write(0xFF);
        delay(25);
      }
      lcd.setCursor(0, 1);
      for (int i = 0; i < LCD_COLS; i++) {
        lcd.write(' ');
        delay(25);
      }

      // print actual result
      printDiceType(0, 1);
      lcd.print(": ");
      lcd.print(value, DEC);
      lcd.print(" ");
      printMurasakiNumber(value, -1, -1);
      delay(500);

      digitalWrite(LED_BUILTIN, LOW);
    }

    void updateMurasakiClock() {
      unsigned long newSeconds = millis() / 1000;
      if (newSeconds != secondsSinceStart) {
        secondsSinceStart = newSeconds;
        printMurasakiNumber(secondsSinceStart, LCD_COLS - 1, 0);
      }
    }
};


void *murasakiSetup() {
  MurasakiApp *app = new MurasakiApp();
  app->setup();
  return app;
}

void murasakiLoop(void *appStatePointer) {
  ((MurasakiApp *) appStatePointer)->loop();
}

void murasakiExit(void *appStatePointer) {
  delete ((MurasakiApp *) appStatePointer);
}

const App murasakiAppDefinition = {
name: "JDR Murasaki",
setup: murasakiSetup,
loop: murasakiLoop,
exit: murasakiExit,
};
