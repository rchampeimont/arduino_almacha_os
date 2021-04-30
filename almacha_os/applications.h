#include "app.h"

extern const App katakanaAppDefinition;
extern const App keyboardTestAppDefinition;

const byte NUMBER_OF_APPLICATIONS = 2;
const App applications[] = {
  katakanaAppDefinition,
  keyboardTestAppDefinition
};
