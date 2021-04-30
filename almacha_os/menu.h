#ifndef MENU_H
#define MENU_H

class Menu {
  public:
    Menu(const char * const* choices, byte numberOfChoices, const char *title);
    void setup();
    void loop();

    // Returns selected value if any, otherwise 0xff
    byte getSelectionMade();

  private:
    const char * const* choices;
    byte numberOfChoices;
    byte currentSelection;
    byte previousSelection;
    void printPage();
    bool selectionMade;
    const char *title;
};

#endif
