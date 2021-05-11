
volatile byte keyboardBitCounter = 0;
volatile byte incompleteKeycode = 0;

volatile unsigned long lastKeyboardIntrruptTime = 0;
volatile byte keyboardInterruptDeltasIndex = 0;
const byte KEYBOARD_INTERRUPT_DELTA_ARRAY_SIZE = 15;
volatile byte keyboardInterruptDeltas[KEYBOARD_INTERRUPT_DELTA_ARRAY_SIZE];


const byte KEYBOARD_BUFFER_SIZE = 4;
volatile byte keyboardBuffer[KEYBOARD_BUFFER_SIZE];
volatile byte keyboardBufferIndex = 0;
volatile byte keyboardErrorStatus = 0;

volatile bool unreadKey = false;
volatile bool unreadKeyByOS = false;
volatile unsigned long lastCombinedCode = 0;

const long AUTOMATIC_RESET_AFTER = 200; // microseconds



void processKeyboardInterrupt() {
  unsigned long currentTime = micros();
  unsigned long delta = currentTime - lastKeyboardIntrruptTime;
  keyboardInterruptDeltas[keyboardInterruptDeltasIndex] = (delta > 255) ? 255 : (byte) delta;
  lastKeyboardIntrruptTime = currentTime;
  keyboardInterruptDeltasIndex++;
  if (keyboardInterruptDeltasIndex >= KEYBOARD_INTERRUPT_DELTA_ARRAY_SIZE) {
    keyboardInterruptDeltasIndex = 0;
  }

  byte data = digitalRead(PS2_KEYBOARD_DATA_PIN);

  // If a long time occured since the last 11-bit sequence, reset to reading a new value.
  // This allows us to get out of a dead end if an extraneous or missing bit was transmitted previously.
  if (delta > AUTOMATIC_RESET_AFTER) {
    incompleteKeycode = 0;
    keyboardBitCounter = 0;
    keyboardErrorStatus = 0;
  }

  if (keyboardErrorStatus) return; // stop keyboard system on error: useful for debug

  if (keyboardBitCounter == 0) {
    // start bit
    if (data != 0) {
      // invalid start bit, it should be 0
      keyboardErrorStatus = 'S';
      reportKeyboardError();
    }
  } else if (keyboardBitCounter > 0 && keyboardBitCounter < 9) {
    // data bit: least significant bit is received first.
    incompleteKeycode |= data << (keyboardBitCounter - 1);
  } else if (keyboardBitCounter == 9) {
    // we have just read parity bit, so let's check parity
    byte parity = data;
    for (byte i = 0; i < 8; i++) {
      parity += incompleteKeycode >> i & 1;
    }
    // odd parity is expected
    if (parity % 2 != 1 && !keyboardErrorStatus) {
      // incorrect parity
      keyboardErrorStatus = 'P';
      reportKeyboardError();
    }
  }

  if (keyboardBitCounter == 10) {
    // We are at stop bit.
    if (data != 1 && !keyboardErrorStatus) {
      // invalid end bit: it should be 1
      keyboardErrorStatus = 'E';
      reportKeyboardError();
    }
    // All 11 bits have been received.
    if (! keyboardErrorStatus) {
      // process received byte if every check passed
      processKeyboard11BitCode();
    }
    // Reset to read next 11-bit chunk
    incompleteKeycode = 0;
    keyboardBitCounter = 0;
  } else {
    keyboardBitCounter++;
  }
}


void processKeyboard11BitCode() {
  // Add keycode to buffer
  keyboardBuffer[keyboardBufferIndex] = incompleteKeycode;
  keyboardBufferIndex++;
  if (keyboardBufferIndex >= KEYBOARD_BUFFER_SIZE) {
    keyboardBufferIndex = 0;
  }
  
  unsigned long combinedCode = tryCombineCodes();
  if (combinedCode) {
    lastCombinedCode = combinedCode;
    unreadKey = true;
    unreadKeyByOS = true;
  }
}

unsigned long tryCombineCodes() {
  byte n1 = keyboardBuffer[(keyboardBufferIndex - 1 + KEYBOARD_BUFFER_SIZE) % KEYBOARD_BUFFER_SIZE];
  byte n2 = keyboardBuffer[(keyboardBufferIndex - 2 + KEYBOARD_BUFFER_SIZE) % KEYBOARD_BUFFER_SIZE];
  byte n3 = keyboardBuffer[(keyboardBufferIndex - 3 + KEYBOARD_BUFFER_SIZE) % KEYBOARD_BUFFER_SIZE];
  
  if (n1 == 0xE0 || n1 == 0xF0) {
    // unfinished code
    return 0;
  } else if (n2 == 0xF0) {
    // key released
    if (n3 == 0xE0) {
      // extended key
      return n1 | 0xE0F000;
    } else {
      // regular key
      return n1 | 0xF000;
    }
  } else if (n2 == 0xE0) {
    // extended key pressed
    return n1 | 0xE000;
  } else {
    // regular key pressed
    return n1;
  }
}

void reportKeyboardError() {
  // print debug information to serial
  Serial.print("Keyboard error: ");
  Serial.write(keyboardErrorStatus);
  Serial.print(" / Partially read key code: ");
  Serial.println(incompleteKeycode, BIN);
}


void initKeyboard() {
  Serial.print("Initializing keyboard... ");
  
  for (int i = 0; i < KEYBOARD_BUFFER_SIZE; i++) {
    keyboardBuffer[i] = 0;
  }

  pinMode(PS2_KEYBOARD_CLOCK_PIN, INPUT);
  pinMode(PS2_KEYBOARD_DATA_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PS2_KEYBOARD_CLOCK_PIN), processKeyboardInterrupt, FALLING);

  Serial.println("OK");
}

// Reads key and marks it as read
unsigned long keyboardReadKey() {
  if (unreadKey) {
    unreadKey = false;
    // Disable interrupts to read reliably a >1 byte variable modified by an ISR
    //noInterrupts();
    unsigned long code = lastCombinedCode;
    if (code) {
      Serial.print("Keyboard key read: ");
      Serial.println(code, HEX);
    }
    //interrupts();
    return code;
  } else {
    // Now new keycodes have been received since last function call
    return 0;
  }
}

// For OS only, it does not flag the key as read, to let an app read it
unsigned long keyboardOSReadKey() {
  if (unreadKeyByOS) {
    unreadKeyByOS = false;
    // Disable interrupts to read reliably a >1 byte variable modified by an ISR
    //noInterrupts();
    unsigned long code = lastCombinedCode;
    if (code) {
      Serial.print("Keyboard key read by OS: ");
      Serial.println(code, HEX);
    }
    //interrupts();
    return code;
  } else {
    // Now new keycodes have been received since last function call
    return 0;
  }
}
