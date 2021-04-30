#ifndef APP_KATAKANA_H
#define APP_KATAKANA_H

class AppKatakana {
  public:
    AppKatakana();
    void setup();
    void loop();
  private:
    byte lastSelectedPage = 0xFF;
    byte selectedPage = 0;
};

#endif
