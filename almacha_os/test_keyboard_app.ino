
void keyboardTestAppLoop() {
  unsigned long key = keyboardReadKey();
  if (key) {
    lcd.clear();
    lcd.print(key, HEX);
  }
}

const struct App keyboardTestAppDefinition = {
name: "Test keyboard",
setup: NULL,
loop: keyboardTestAppLoop,
exit: NULL,
};
