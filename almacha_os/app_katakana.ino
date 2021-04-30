#include <LiquidCrystal.h>
#include "app_katakana.h"

extern const int LCD_COLS;

const int ARRAY_SIZE = 56;
const int KANAS_PER_PAGE = 4;
const int CHARS_PER_KANA = 4;
const int N_PAGES = ARRAY_SIZE/KANAS_PER_PAGE;
const char* translations[ARRAY_SIZE] = {
  "Wo",
  "a", "i", "u", "e", "o",
  "ya", "yu", "yo", "tsu",
  "-",
  "A", "I", "U", "E", "O",
  "Ka", "Ki", "Ku", "Ke", "Ko",
  "Sa", "Shi", "Su", "Se", "So",
  "Ta", "Chi", "Tsu", "Te", "To",
  "Na", "Ni", "Nu", "Ne", "No",
  "Ha", "Hi", "Fu", "He", "Ho",
  "Ma", "Mi", "Mu", "Me", "Mo",
  "Ya", "Yu", "Yo",
  "Ra", "Ri", "Ru", "Re", "Ro",
  "Wa", "N",
};



AppKatakana::AppKatakana() {
  lastSelectedPage = 0xFF;
  selectedPage = 0;
}

void AppKatakana::setup() {
}

void AppKatakana::loop() {
  unsigned long key = keyboardReadKey();
  if (key == 0xE074 || key == 0xE072) {
    // right or down arrow
    selectedPage++;
    if (selectedPage >= N_PAGES) {
      selectedPage = 0;
    }
  } else if (key == 0xE06B || key == 0xE075) {
    // left or up arrow
    selectedPage--;
    if (selectedPage >= N_PAGES) {
      selectedPage = N_PAGES-1;
    }
  }
  
  if (lastSelectedPage != selectedPage) {
    // page changed
    Serial.println("Katakana page changed");
    lastSelectedPage = selectedPage;
    
    lcd.clear();

    // Display katakanas and their translation
    for (int i=0; i<KANAS_PER_PAGE; i++) {
      int index = selectedPage*KANAS_PER_PAGE + i;
      lcd.setCursor(i*CHARS_PER_KANA, 0);
      lcd.print(translations[index]);
      lcd.setCursor(i*CHARS_PER_KANA, 1);
      lcd.write(0xA6 + index);
    }

    // Display page number
    int pageNumber = selectedPage + 1;
    lcd.setCursor(LCD_COLS - (pageNumber < 10 ? 1 : 2), 1);
    lcd.print(pageNumber, DEC);
  }
}
