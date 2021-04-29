// Copyright 2021 Raphael Champeimont

#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

extern void initKeyboard();
extern void reportKeyboardRawData();

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

void loop() {
  //reportKeyboardRawData();
  appKatakanaLoop();
}
