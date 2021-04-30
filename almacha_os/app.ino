#include "app.h"

App::App() {
  
}


void App::setup() {
  lcd.clear();
  lcd.write("empty app setup");
}

void App::loop() {
  lcd.clear();
  lcd.write("empty app loop");
}
