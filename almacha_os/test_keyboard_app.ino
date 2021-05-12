
void keyboardTestAppLoop(void *state) {
  unsigned long key = keyboardReadKey();
  if (key) {
    lcd.clear();
    lcd.print(key, HEX);
    lcd.setCursor(0, 1);
    lcd.write("locks state: ");
    lcd.print(keyboardGetLockState(), BIN);
  }
}

void keyboardTestAppSetup() {
  lcd.clear();
  lcd.write("Press any key");
}

const App keyboardTestAppDefinition = {
name: "Test keyboard",
setup: keyboardTestAppSetup,
loop: keyboardTestAppLoop,
exit: NULL,
};
