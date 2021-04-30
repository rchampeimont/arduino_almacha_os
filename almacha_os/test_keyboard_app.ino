
void keyboardTestAppLoop(void *state) {
  unsigned long key = keyboardReadKey();
  if (key) {
    lcd.clear();
    lcd.print(key, HEX);
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
