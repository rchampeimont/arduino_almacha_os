#ifndef MENU_H
#define MENU_H

class Menu {
  public:
    Menu(const char * const* choices, byte numberOfChoices, const char *title);
    void setup();
    void loop();
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
