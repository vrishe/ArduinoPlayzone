#define PIN_CHANNEL_R (11)
#define PIN_CHANNEL_G (10)
#define PIN_CHANNEL_B ( 9)

void displayColor(byte r, byte g, byte b) {
  analogWrite(PIN_CHANNEL_R, r);
  analogWrite(PIN_CHANNEL_G, g);
  analogWrite(PIN_CHANNEL_B, b);
}


#define PIN_LED_SIGNAL 13

void yieldError(unsigned char code) {
  if (code == 0) {
    digitalWrite(13, HIGH);
    
    while(true) { /*  */ }
  }
  int shift = 0;
  
  do {
    if (shift == (sizeof(char) << 3)) {
      shift = 0;
      
      delay(2000);
    }
    int duration = !!((code << shift++) & 0x80) ? 750 : 250;
    
    digitalWrite(13, HIGH);
    delay(duration);
    digitalWrite(13, LOW);
    delay(250);  
  } while(true);
}

void setup() {
  pinMode(PIN_LED_SIGNAL, OUTPUT);
  digitalWrite(13, LOW);  
  
  Serial.begin(115200);
  
  pinMode(PIN_CHANNEL_R, OUTPUT);
  pinMode(PIN_CHANNEL_G, OUTPUT);
  pinMode(PIN_CHANNEL_B, OUTPUT);

  displayColor(0, 0, 0);
  
  Serial.write("Please, enter colors to interpolate through.\n");
}


#define PARSE_HEX_DIGIT(digit)                                   \
  (('0' <= (digit) && (digit) <= '9') ? (digit) - '0'            \
    : (('a' <= (digit) && (digit) <= 'f') ? (digit) - 'a' + 10   \
      : (('A' <= (digit) && (digit) <= 'F') ? (digit) - 'A' + 10 \ 
        : 0)))

byte parseByteHex(const char *buffer) { 
  return (PARSE_HEX_DIGIT(*buffer) << 4) + PARSE_HEX_DIGIT(*(buffer + 1));
}

static char buffer[6];
static char *ptrBuffer;

static bool isPlaying;
static bool isReading;

static int colorIndex;
static int colorsCount;
static byte colorBuffer[16][3];

#define TRANSITION_TIME_DURATION (4096)

void loop() { 
  static long start;
  
  if (Serial.available()) {
    if (!isReading) {
      int valueRead = Serial.read();
      
      if (valueRead == '.') {
        isPlaying = !isPlaying;
        
        if (isPlaying) {
          Serial.println("Done.");
          
          colorIndex = 0;
          start = millis();
        } else {
          colorsCount = 0;
          displayColor(0, 0, 0);
          
          Serial.write("Please, enter colors to interpolate through.\n");
        }
      } else if (!isPlaying && valueRead == '#') {
        isReading = true;
        ptrBuffer = buffer;
      }
    } else {
      *ptrBuffer++ = (char)Serial.read();
      
      if (ptrBuffer >= buffer + sizeof(buffer)) {
        isReading = false;
         
        displayColor(              
          colorBuffer[colorsCount][0] = parseByteHex(buffer + 0),
          colorBuffer[colorsCount][1] = parseByteHex(buffer + 2),
          colorBuffer[colorsCount][2] = parseByteHex(buffer + 4)
        );
        colorsCount++;
        
        Serial.write("Color "); Serial.print(colorsCount); Serial.write(": #");
        Serial.print(colorBuffer[colorsCount - 1][0], HEX);
        Serial.print(colorBuffer[colorsCount - 1][1], HEX);
        Serial.print(colorBuffer[colorsCount - 1][2], HEX);
        Serial.write('\n');
      }
    }
  }
  if (isPlaying && colorsCount > 0) {
    long delta = millis() - start;
    
    int colorIndexNext = (colorIndex + 1) % colorsCount;
    
    if (delta < TRANSITION_TIME_DURATION) {    
      int aR = colorBuffer[colorIndex][0]; int dR = (int)colorBuffer[colorIndexNext][0] - aR;
      int aG = colorBuffer[colorIndex][1]; int dG = (int)colorBuffer[colorIndexNext][1] - aG;
      int aB = colorBuffer[colorIndex][2]; int dB = (int)colorBuffer[colorIndexNext][2] - aB;

      float bias = delta / (float)TRANSITION_TIME_DURATION;
      
      displayColor(
        aR + round(dR * bias),
        aG + round(dG * bias),
        aB + round(dB * bias)
      );
      delay(160);
    } else {    
      colorIndex = colorIndexNext;
      start = millis();
    }
  }
}
