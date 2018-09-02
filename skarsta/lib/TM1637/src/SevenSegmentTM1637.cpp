
#include "SevenSegmentTM1637.h"

// store an ASCII Map in PROGMEM (Flash memory)
const PROGMEM uint8_t asciiMap[96] = {
  TM1637_CHAR_SPACE,
  TM1637_CHAR_EXC,
  TM1637_CHAR_D_QUOTE,
  TM1637_CHAR_POUND,
  TM1637_CHAR_DOLLAR,
  TM1637_CHAR_PERC,
  TM1637_CHAR_AMP,
  TM1637_CHAR_S_QUOTE,
  TM1637_CHAR_L_BRACKET,
  TM1637_CHAR_R_BRACKET,
  TM1637_CHAR_STAR,
  TM1637_CHAR_PLUS,
  TM1637_CHAR_COMMA,
  TM1637_CHAR_MIN,
  TM1637_CHAR_DOT,
  TM1637_CHAR_F_SLASH,
  TM1637_CHAR_0,          // 48 (ASCII)
  TM1637_CHAR_1,
  TM1637_CHAR_2,
  TM1637_CHAR_3,
  TM1637_CHAR_4,
  TM1637_CHAR_5,
  TM1637_CHAR_6,
  TM1637_CHAR_7,
  TM1637_CHAR_8,
  TM1637_CHAR_9,
  TM1637_CHAR_COLON,
  TM1637_CHAR_S_COLON,
  TM1637_CHAR_LESS,
  TM1637_CHAR_EQUAL,
  TM1637_CHAR_GREAT,
  TM1637_CHAR_QUEST,
  TM1637_CHAR_AT,
  TM1637_CHAR_A,          // 65 (ASCII)
  TM1637_CHAR_B,
  TM1637_CHAR_C,
  TM1637_CHAR_D,
  TM1637_CHAR_E,
  TM1637_CHAR_F,
  TM1637_CHAR_G,
  TM1637_CHAR_H,
  TM1637_CHAR_I,
  TM1637_CHAR_J,
  TM1637_CHAR_K,
  TM1637_CHAR_L,
  TM1637_CHAR_M,
  TM1637_CHAR_N,
  TM1637_CHAR_O,
  TM1637_CHAR_P,
  TM1637_CHAR_Q,
  TM1637_CHAR_R,
  TM1637_CHAR_S,
  TM1637_CHAR_T,
  TM1637_CHAR_U,
  TM1637_CHAR_V,
  TM1637_CHAR_W,
  TM1637_CHAR_X,
  TM1637_CHAR_Y,
  TM1637_CHAR_Z,
  TM1637_CHAR_L_S_BRACKET,  // 91 (ASCII)
  TM1637_CHAR_B_SLASH,
  TM1637_CHAR_R_S_BRACKET,
  TM1637_CHAR_A_CIRCUM,
  TM1637_CHAR_UNDERSCORE,
  TM1637_CHAR_A_GRAVE,       // 96 (ASCII)
  TM1637_CHAR_a,
  TM1637_CHAR_b,
  TM1637_CHAR_c,
  TM1637_CHAR_d,
  TM1637_CHAR_e,
  TM1637_CHAR_f,
  TM1637_CHAR_g,
  TM1637_CHAR_h,
  TM1637_CHAR_i,
  TM1637_CHAR_j,
  TM1637_CHAR_k,
  TM1637_CHAR_l,
  TM1637_CHAR_m,
  TM1637_CHAR_n,
  TM1637_CHAR_o,
  TM1637_CHAR_p,
  TM1637_CHAR_q,
  TM1637_CHAR_r,
  TM1637_CHAR_s,
  TM1637_CHAR_t,
  TM1637_CHAR_u,
  TM1637_CHAR_v,
  TM1637_CHAR_w,
  TM1637_CHAR_x,
  TM1637_CHAR_y,
  TM1637_CHAR_z,
  TM1637_CHAR_L_ACCON,  // 123 (ASCII)
  TM1637_CHAR_BAR,
  TM1637_CHAR_R_ACCON,
  TM1637_CHAR_TILDE     // 126 (ASCII)
};


SevenSegmentTM1637::SevenSegmentTM1637(uint8_t pinClk, uint8_t pinDIO) :
  _pinClk(pinClk),
  _pinDIO(pinDIO)
{
  // setup pins
  pinAsOutput(_pinClk);
  pinAsOutput(_pinDIO);
  digitalHigh(_pinClk);
  digitalHigh(_pinDIO);

  // setup defaults
  setCursor(0, TM1637_DEFAULT_CURSOR_POS);
  setPrintDelay(TM1637_DEFAULT_PRINT_DELAY);
  setColonOn(TM1637_DEFAULT_COLON_ON);
  setBacklight(TM1637_DEFAULT_BACKLIGHT);

  // write command SET_DATA   (Command1) Defaults
  command(
    TM1637_COM_SET_DATA |
    TM1637_SET_DATA_WRITE |
    TM1637_SET_DATA_A_ADDR |
    TM1637_SET_DATA_M_NORM
  );



};

// Print API ///////////////////////////////////////////////////////////////////
// single byte
size_t  SevenSegmentTM1637::write(uint8_t byte) {
  TM1637_DEBUG_PRINT(F("write byte:\t")); TM1637_DEBUG_PRINTLN(byte);

  if ( _cursorPos == _numCols ) {
    shiftLeft(_rawBuffer, _numCols);
    _rawBuffer[_cursorPos] = encode( (char)byte );
    printRaw( _rawBuffer, _numCols, 0);
  };

  if (_cursorPos < _numCols) {
    _rawBuffer[_cursorPos] = encode( (char)byte );
    printRaw( _rawBuffer, _cursorPos+1, 0);
    setCursor(1, _cursorPos + 1);
  };

}

// null terminated char array
size_t  SevenSegmentTM1637::write(const char* str) {
  TM1637_DEBUG_PRINT(F("write char*:\t")); TM1637_DEBUG_PRINTLN(str);
  uint8_t encodedBytes[4];

  encode(encodedBytes, str, 4);
  uint8_t i =4;
  while( str[i] != '\0' ) {
    printRaw(encodedBytes);
    shiftLeft(encodedBytes, 4);
    encodedBytes[3] = encode( str[i] );
    i++;
    if ( i == TM1637_MAX_CHARS) {
      break;
    }
  }
};

// byte array with length
size_t  SevenSegmentTM1637::write(const uint8_t* buffer, size_t size) {
  TM1637_DEBUG_PRINT(F("write uint8_t*:\t")); TM1637_DEBUG_PRINT(buffer[0]);
  uint8_t encodedBytes[TM1637_MAX_CHARS];

  if ( size > TM1637_MAX_CHARS) {
    size = TM1637_MAX_CHARS;
  }
  size_t length = encode(encodedBytes, buffer, size);
  TM1637_DEBUG_PRINT(F(" ")); TM1637_DEBUG_PRINTLN(encodedBytes[0], BIN);
  printRaw(encodedBytes,length, _cursorPos);
};

// Liquid cristal API
void SevenSegmentTM1637::begin(uint8_t cols, uint8_t rows) {
  _numCols = cols;
  _numRows = rows;
  clear();
};

void SevenSegmentTM1637::init(uint8_t cols, uint8_t rows) {
  begin(cols, rows);
}

void SevenSegmentTM1637::clear(void) {
  uint8_t rawBytes[4] = {0,0,0,0};
  printRaw(rawBytes);
  home();
};

void SevenSegmentTM1637::home(void) {
  setCursor(0, 0);
};

void SevenSegmentTM1637::setCursor(uint8_t row, uint8_t col) {
  col = (col > TM1637_MAX_COLOM -1)?TM1637_MAX_COLOM-1:col;
  _cursorPos = col;

};

void SevenSegmentTM1637::setBacklight(uint8_t value) {
  value = (value    > 100 )?100:value;  // 0..100 brightness
  // Store the backlight value
  _backLightValue = value;
  // scale backlight value to 0..8
  value /= 10;                          // 0..10
  value = (value > 8   )?  8:value;     // only 8 levels and off
  uint8_t cmd = TM1637_COM_SET_DISPLAY;;
  switch ( value ) {
    case 0:
      cmd |= TM1637_SET_DISPLAY_OFF;
      break;
    case 1:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_1;
      break;
    case 2:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_2;
      break;
    case 3:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_4;
      break;
    case 4:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_10;
      break;
    case 5:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_11;
      break;
    case 6:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_12;
      break;
    case 7:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_13;
      break;
    case 8:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_14;
      break;
    };
    bool ack = command(cmd);
    TM1637_DEBUG_PRINT(F("SET_DISPLAY:\t")); TM1637_DEBUG_PRINTLN((
      cmd
    ), BIN);
    TM1637_DEBUG_PRINT(F("Acknowledged:\t")); TM1637_DEBUG_PRINTLN(ack);
};

void SevenSegmentTM1637::setContrast(uint8_t value) {
  setBacklight(value);
}

void SevenSegmentTM1637::on(void) {
  setBacklight(TM1637_DEFAULT_BACKLIGHT);
};

void SevenSegmentTM1637::off(void) {
  setBacklight(0);
  clear();
};

// SevenSegmentTM1637 public methods
void  SevenSegmentTM1637::blink(uint8_t blinkDelay, uint8_t repeats, uint8_t maxBacklight, uint8_t minBacklight) {
  for (uint8_t i=0; i < repeats; i++) {
    setBacklight(minBacklight);     // turn backlight off
    delay(blinkDelay);
    setBacklight(maxBacklight);     // turn backlight on
    delay(blinkDelay);
  }
  // restore backlight
  setBacklight(_backLightValue);
}

void  SevenSegmentTM1637::setPrintDelay(uint16_t printDelay) {
  _printDelay = printDelay;
};

bool  SevenSegmentTM1637::getColonOn(void) {
  return (_colonOn);
};

void  SevenSegmentTM1637::setColonOn(bool setToOn) {
    _colonOn = setToOn;
}
void  SevenSegmentTM1637::printRaw(uint8_t rawByte, uint8_t position) {
  uint8_t cmd[2];
  cmd[0] = TM1637_COM_SET_ADR | position;
  cmd[1] = rawByte;
  if (position == 1) { cmd[1]|=(_colonOn)?TM1637_COLON_BIT:0; };
  command(cmd, 2);
};

void  SevenSegmentTM1637::printRaw(const uint8_t* rawBytes, size_t length, uint8_t position) {

  // if fits on display
  if ( (length + position) <= _numCols) {
    uint8_t cmd[5] = {0, };
    cmd[0] = TM1637_COM_SET_ADR | (position & B111);  // sets address
    memcpy(&cmd[1], rawBytes, length);       // copy bytes

    // do we have to print a colon?
    if ( position < 2 ) { // printing after position 2 has never a colon
      if ( position == 0 && length >= 2) {
        // second index is the colon
        cmd[2] |= (_colonOn)?TM1637_COLON_BIT:0;
      } else {
        // first index is the colon
        cmd[1] |= (_colonOn)?TM1637_COLON_BIT:0;
      }
    }
    TM1637_DEBUG_PRINT(F("ADDR :\t")); TM1637_DEBUG_PRINTLN(cmd[0],BIN);
    TM1637_DEBUG_PRINT(F("DATA0:\t")); TM1637_DEBUG_PRINTLN(cmd[1],BIN);
    command(cmd, length+1);                           // send to display
  }
  // does not fit on display, need to print with delay
  else {
    // First print 1-4 characters
    uint8_t numtoPrint = _numCols - position;
    printRaw(rawBytes, numtoPrint, position);
    delay(_printDelay);

    // keep printing 4 characters till done
    uint8_t remaining = length - numtoPrint + 3;
    uint8_t i         = 1;
    while( remaining >= _numCols) {
      printRaw(&rawBytes[i], _numCols, 0);
      delay(_printDelay);
      remaining--;
      i++;
    };
  }

};

// Helpers
uint8_t SevenSegmentTM1637::encode(char c) {
  if ( c < ' ') { // 32 (ASCII)
    return 0;
  }
  return pgm_read_byte_near(asciiMap + c - ' ');
};

uint8_t SevenSegmentTM1637::encode(int16_t d) {
  // can only encode single digit
  if ( d > 9 || d < 0) {
    return 0;
  };
  return pgm_read_byte_near(asciiMap + d + '0' - ' ');
};

size_t  SevenSegmentTM1637::encode(uint8_t* buffer, const char* str, size_t bufferSize) {
  size_t i;

  for (i=0; i < bufferSize; i++) {
    if (str[i] == '\0' ) {
      return i;
    }
    buffer[i] = encode( str[i] );
  };
  return i;
}

size_t  SevenSegmentTM1637::encode(uint8_t* buffer, const uint8_t* byteArr, size_t bufferSize) {
  size_t i;

  for (i=0; i < bufferSize; i++) {
    buffer[i] = encode( (char)byteArr[i] );
  };
  return i;
}

void    SevenSegmentTM1637::shiftLeft(uint8_t* buffer, size_t length) {
  for (uint8_t i=0; i < length ; i++) {
    buffer[i] = buffer[i+1];
  }
}

// SevenSegmentTM1637 LOW LEVEL
bool    SevenSegmentTM1637::command(uint8_t cmd) const{
  return command(_pinClk, _pinDIO, cmd);
};

bool    SevenSegmentTM1637::command(uint8_t pinClk, uint8_t pinDIO, uint8_t cmd) {
  comStart(pinClk, pinDIO);
  comWriteByte(pinClk, pinDIO,cmd);
  bool acknowledged = comAck(pinClk, pinDIO);
  comStop(pinClk, pinDIO);
  return acknowledged;
}

bool    SevenSegmentTM1637::command(const uint8_t* commands, uint8_t length) const {
  return command(_pinClk, _pinDIO, commands, length);
};

bool    SevenSegmentTM1637::command(uint8_t pinClk, uint8_t pinDIO, const uint8_t* commands, uint8_t length) {
  bool acknowledged = true;
  comStart(pinClk, pinDIO);
  for (uint8_t i=0; i < length;i++) {
    comWriteByte(pinClk, pinDIO, commands[i]);
    acknowledged &= comAck(pinClk, pinDIO);
  };
  comStop(pinClk, pinDIO);
  return acknowledged;
}

uint8_t SevenSegmentTM1637::comReadByte(void) const {
  uint8_t readKey = 0;

  comStart();
  comWriteByte(TM1637_COM_SET_DATA | TM1637_SET_DATA_READ);
  comAck();

  pinAsInput(_pinDIO);
  digitalHigh(_pinDIO);
  delayMicroseconds(5);

  for ( uint8_t i=0; i < 8; i++) {

    readKey >>= 1;
    digitalLow(_pinClk);
    delayMicroseconds(30);

    digitalHigh(_pinClk);

    if ( isHigh(_pinDIO) ) {
      readKey = readKey | B1000000;
    };

    delayMicroseconds(30);


  };
  pinAsOutput(_pinDIO);
  comAck();
  comStop();
  return readKey;
};

void    SevenSegmentTM1637::comWriteByte(uint8_t command) const{
  comWriteByte(_pinClk, _pinDIO, command);
};

void    SevenSegmentTM1637::comWriteByte(uint8_t pinClk, uint8_t pinDIO, uint8_t command) {
  // CLK in bits
  for ( uint8_t i=0; i < 8; i++) {
    digitalLow(pinClk);   // CLK LOW

    if ( command & B1) {
      digitalHigh(pinDIO);// DIO HIGH
    } else {
      digitalLow(pinDIO); // DIO LOW
    }
    delayMicroseconds(TM1637_CLK_DELAY_US);

    command >>= 1;

    digitalHigh(pinClk);   // CLK HIGH
    delayMicroseconds(TM1637_CLK_DELAY_US);
  };
}

void    SevenSegmentTM1637::comStart(void) const {
  comStart(_pinClk, _pinDIO);
};

void    SevenSegmentTM1637::comStart(uint8_t pinClk, uint8_t pinDIO) {
  digitalHigh(pinDIO);   // DIO HIGH
  digitalHigh(pinClk);   // CLK HIGH
  delayMicroseconds(TM1637_CLK_DELAY_US);

  digitalLow(pinDIO);    // DIO  LOW
}

void    SevenSegmentTM1637::comStop(void) const {
  comStop(_pinClk, _pinDIO);
};

void    SevenSegmentTM1637::comStop(uint8_t pinClk, uint8_t pinDIO) {
  digitalLow(pinClk);   // CLK LOW
  delayMicroseconds(TM1637_CLK_DELAY_US);

  digitalLow(pinDIO);    // DIO LOW
  delayMicroseconds(TM1637_CLK_DELAY_US);

  digitalHigh(pinClk);   // CLK HIGH
  delayMicroseconds(TM1637_CLK_DELAY_US);

  digitalHigh(pinDIO);   // DIO HIGH
}

bool    SevenSegmentTM1637::comAck(void) const {
  comAck(_pinClk, _pinDIO);
};

bool    SevenSegmentTM1637::comAck(uint8_t pinClk, uint8_t pinDIO) {
  bool acknowledged = false;

  digitalLow(pinClk);          // CLK  LOW
  pinAsInputPullUp(pinDIO);    // DIO INPUT PULLUP (state==HIGH)
  delayMicroseconds(TM1637_CLK_DELAY_US);

  acknowledged = isLow(pinDIO);// Ack should pull the pin low again

  digitalHigh(pinClk);         // CLK HIGH
  delayMicroseconds(TM1637_CLK_DELAY_US);

  digitalLow(pinClk);          // CLK  LOW
  pinAsOutput(pinDIO);

  return acknowledged;
}
