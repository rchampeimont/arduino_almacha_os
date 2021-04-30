// Copyright 2021 Raphael Champeimont

#include <LiquidCrystal.h>
#include "menu.h"

extern LiquidCrystal lcd;
extern void initKeyboard();

byte runningApp = 0xff;
byte previousLoopRunningApp = 0;

const byte numberOfApps = 3;
const char* appNames[numberOfApps] = { "Katakana", "coucou", "cheval" };
Menu *mainMenu = NULL;

void setup() {
  // Set up internal LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Switch on internal LED for debug
  digitalWrite(LED_BUILTIN, HIGH);
  
  // Debug output will be sent to serial
  Serial.begin(9600);
  Serial.println("Starting system...");

  initLCD();
  initKeyboard();

  // Report system ready
  lcd.home();
  lcd.print("System ready.");
  Serial.println("System ready.");
  digitalWrite(LED_BUILTIN, LOW);
}

bool appNeedsInit() {
  bool result;
  result = previousLoopRunningApp != runningApp;
  previousLoopRunningApp = runningApp;
  return result;
}

void loop() {
  unsigned long key = keyboardOSReadKey();
  if (key == 0x76) {
    // Escape pressed, go back to main menu
    Serial.println("OS: Escape pressed. Going back to main menu.");
    runningApp = 0xff;
  }
  
  switch (runningApp) {
    case 0:
      appKatakanaLoop();
      break;
    default:
      if (appNeedsInit()) {
        mainMenu = new Menu(appNames, numberOfApps);
      }
      mainMenu->loop();
      int selection = mainMenu->getSelectionMade();
      if (selection != 0xff) {
        // a selection was made
        runningApp = selection;
        delete mainMenu;
        mainMenu = NULL;
        Serial.print("OS: Changed running app to: ");
        Serial.println(runningApp);
      }
  }
}
