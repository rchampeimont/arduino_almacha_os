// Copyright 2021 Raphael Champeimont

#include <LiquidCrystal.h>
#include "menu.h"
#include "applications.h"
#include "keyboard.h"

extern LiquidCrystal lcd;
extern const int LCD_COLS;
extern void initLCD();

const char* appNames[NUMBER_OF_APPLICATIONS];
Menu *mainMenu = NULL;
App *runningApp = NULL;
void* appStatePointer = NULL;

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

  // Get app names
  for (byte i=0; i<NUMBER_OF_APPLICATIONS; i++) {
    appNames[i] = applications[i].name;
  }

  // Report system ready
  Serial.println("System ready.");

  // Welcome message
  lcd.write("Welcome to");
  lcd.setCursor(LCD_COLS-10, 1);
  lcd.write("Almacha OS");
  delay(1500);

  digitalWrite(LED_BUILTIN, LOW);
}

void startApp(byte selectedApp) {
  runningApp = &applications[selectedApp];
  
  Serial.print("OS: Starting app: ");
  Serial.println(runningApp->name);

  Serial.print("OS: Running app setup... ");
  if (runningApp->setup) {
    appStatePointer = runningApp->setup();
    Serial.println("OK");
  } else {
    appStatePointer = NULL;
    Serial.println("no setup provided");
  }

  // Clear possible pending key to read (to prevent app from reading a pending ENTER for instancee)
  keyboardReadKey();
}

void stopApp() {
  Serial.print("OS: Escape key pressed. Exiting app... ");
  if (runningApp->exit) {
    runningApp->exit(appStatePointer);
    Serial.println("OK");
  } else {
    Serial.println("no exit procedure needed");
  }
  
  runningApp = NULL;
  appStatePointer = NULL;
}

void loop() {
  // Read keyboard input
  unsigned long key = keyboardOSReadKey();
  
  if (runningApp) {
    // We are in an app
    if (key == 0x76) {
      // Escape pressed, so stop app
      stopApp();
    } else {
      // Run app loop
      runningApp->loop(appStatePointer);
    }
  } else {
    // We are main menu
    if (! mainMenu) {
      // We just came back from an app to the main menu.
      // Reset LCD screen, which allows to recover usage
      // of the LCD screen if it got disconnected or dysfunctionned.
      initLCD();
      mainMenu = new Menu(appNames, NUMBER_OF_APPLICATIONS, "Select app:");
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
