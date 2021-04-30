#include "menu.h"

Menu::Menu(const char * const* choices, byte numberOfChoices, const char *title) {
  this->choices = choices;
  this->numberOfChoices = numberOfChoices;
  this->title = title;
  currentSelection = 0;
  previousSelection = 0xFF;
  selectionMade = false;
}

void Menu::loop() {
  unsigned long key = keyboardReadKey();
  if (key == 0xE072) {
    currentSelection++;
    if (currentSelection >= numberOfChoices) {
      currentSelection = 0;
    }
  } else if (key == 0xE075) {
    if (currentSelection == 0) {
      currentSelection = numberOfChoices - 1;
    } else {
      currentSelection--;
    }
  } else if (key == 0x5A) {
    Serial.print("Menu selection was made: ");
    Serial.print(currentSelection);
    Serial.println("");
    selectionMade = true;
  }
  
  if (previousSelection != currentSelection) {
    previousSelection = currentSelection;
    printPage();
  }
}

void Menu::printPage() {
  lcd.clear();
  for (byte i=0; i<LCD_ROWS && i<numberOfChoices; i++) {
    lcd.setCursor(0, i);
    if (i == 0) {
      lcd.write("\x7E "); // 0x7E is right arrow
    } else {
      lcd.write("  ");
    }
    lcd.print(choices[(currentSelection + i) % numberOfChoices]);
  }
}

byte Menu::getSelectionMade() {
  if (selectionMade) {
    return currentSelection;
  } else {
    return 0xff;
  }
}
