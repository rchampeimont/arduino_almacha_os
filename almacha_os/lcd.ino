

LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_DATA_PIN_4, LCD_DATA_PIN_5, LCD_DATA_PIN_6, LCD_DATA_PIN_7);

void initLCD() {
  // Init LCD screen
  Serial.print("Initializing LCD screen... ");
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  Serial.println("OK");
}
