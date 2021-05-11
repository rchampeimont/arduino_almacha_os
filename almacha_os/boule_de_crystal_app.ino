#include <LiquidCrystal.h>

class BouleDeCrystal {
  public:
    void setup() {
      lcd.clear();
      lcd.print("Demandez a la");
      lcd.setCursor(0, 1);
      lcd.print("boule de crystal !");
    }

    void loop() {
      unsigned long key = keyboardReadKey();

      if (key == 0x5A) {
        reply = random(8);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("La boule dit :");
        lcd.setCursor(0, 1);
        switch (reply) {
          case 0:
            lcd.print("Oui");
            break;
          case 1:
            lcd.print("Quasiment sur");
            break;
          case 2:
            lcd.print("Certainement");
            break;
          case 3:
            lcd.print("Probable");
            break;
          case 4:
            lcd.print("Je ne sais pas");
            break;
          case 5:
            lcd.print("Repetez SVP");
            break;
          case 6:
            lcd.print("Peu probable");
            break;
          case 7:
            lcd.print("Non");
            break;
        }
      }
    }

  private:
    int switchState = 0;
    int prevSwitchState = 0;
    int reply;
};


void *bouleDeCrystalSetup() {
  BouleDeCrystal *app = new BouleDeCrystal();
  app->setup();
  return app;
}

void bouleDeCrystalLoop(void *appStatePointer) {
  ((BouleDeCrystal *) appStatePointer)->loop();
}

void bouleDeCrystalExit(void *appStatePointer) {
  delete ((BouleDeCrystal *) appStatePointer);
}

const App bouleDeCrystalAppDefinition = {
name: "BouleDeCrystal",
setup: bouleDeCrystalSetup,
loop: bouleDeCrystalLoop,
exit: bouleDeCrystalExit,
};
