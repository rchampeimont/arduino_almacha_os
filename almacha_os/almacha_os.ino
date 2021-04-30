// Copyright 2021 Raphael Champeimont

#include <LiquidCrystal.h>
#include "menu.h"
#include "app_katakana.h"

extern LiquidCrystal lcd;
extern const int LCD_COLS;
extern void initKeyboard();

const byte numberOfApps = 3;
const char* appNames[numberOfApps] = { "Katakana", "coucou", "cheval" };
Menu *mainMenu = NULL;
AppKatakana *runningApp = NULL;

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

  // Welcome message
  lcd.home();
  lcd.write("Welcome to");
  lcd.setCursor(LCD_COLS-10, 1);
  lcd.write("Almacha OS");
  delay(2000);

  // Report system ready
  Serial.println("System ready.");
  digitalWrite(LED_BUILTIN, LOW);
}

void startApp(byte selectedApp) {
  Serial.print("OS: Starting app ");
  Serial.println(selectedApp);

  if (selectedApp == 0) {
    runningApp = new AppKatakana();
  } else {
    return;
  }

  runningApp->setup();
}

void stopApp() {
  Serial.println("OS: Escape key pressed. Exiting app.");
  delete runningApp;
  runningApp = NULL;
}

void loop() {
  if (runningApp) {
    // We are in an app

    // Detect if ESC is pressed to exit app
    unsigned long key = keyboardOSReadKey();
    if (key == 0x76) {
      // Escape pressed, so stop app
      stopApp();
    }

    // Run app loop
    runningApp->loop();
  } else {
    // We are main menu
    if (! mainMenu) {
      mainMenu = new Menu(appNames, numberOfApps, "Select app:");
      mainMenu->setup();
    }
    mainMenu->loop();
    int selection = mainMenu->getSelectionMade();
    if (selection != 0xff) {
      // a selection was made
      delete mainMenu;
      mainMenu = NULL;
      startApp(selection);
    }
  }
}
