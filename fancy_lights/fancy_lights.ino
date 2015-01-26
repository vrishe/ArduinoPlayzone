
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/io.h>

#include <IRremote.h>

#define LIGHT_DURATION_DELTA    128
#define LIGHT_DURATION_MIN      LIGHT_DURATION_DELTA
#define LIGHT_DURATION_INITIAL  LIGHT_DURATION_DELTA * 10
#define LIGHT_DURATION_MAX      LIGHT_DURATION_INITIAL * 2 - LIGHT_DURATION_MIN


//#define DEBUG

static unsigned lightDuration = LIGHT_DURATION_INITIAL;

static const uint8_t diodes[] = { 5, 6, 9 };

static unsigned  diodeState       = 0;
static unsigned  diodeStart[3]    = { -1, -1, -1 };
static boolean   diodeComplete[3] = {  0,  0,  0 };

static void illuminationInitialize() {
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(CS01) | _BV(CS00);
  
  TCCR1A = _BV(COM1A1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10);
  
  digitalWrite(13, HIGH);
  setup();
}

static void illuminationReset() {
  digitalWrite(diodes[0], LOW);
  digitalWrite(diodes[1], LOW);
  digitalWrite(diodes[2], LOW);
}

static void illumination(unsigned duration) {
  static unsigned step = 0;
  static uint8_t diodeIndexNext = 0;
  
  unsigned light_cycle, strobe_offset = duration / 4;
  
  if (diodeState % 2) {
    uint8_t diodeFirst = diodeIndexNext % 3;
    
    if (!diodeComplete[diodeFirst]) {
      if (diodeStart[diodeFirst] == -1) {
        diodeStart[diodeFirst] = step;
      }
      if (diodeLight(diodeFirst, step, diodeStart[diodeFirst], duration)) {
        diodeStart[diodeFirst] = -1;
        
        diodeComplete[diodeFirst] = 1;
      }
    }
    uint8_t diodeSecond = (diodeIndexNext + 1) % 3;
    
    if (!diodeComplete[diodeSecond]) {
      if (diodeStart[diodeSecond] == -1 && (step - diodeStart[diodeFirst]) > strobe_offset) {
        diodeStart[diodeSecond] = step;
      }
      if (diodeStart[diodeSecond] != -1 && diodeLight(diodeSecond, step, diodeStart[diodeSecond], duration)) {
        diodeStart[diodeSecond] = -1;
        
        diodeComplete[diodeSecond] = 1;
      }
    }
    uint8_t diodeThird = (diodeIndexNext + 2) % 3;
    
    if (!diodeComplete[diodeThird]) {
        if (diodeStart[diodeThird] == -1 && diodeStart[diodeSecond] != -1 && (step - diodeStart[diodeSecond]) > strobe_offset) {
          diodeStart[diodeThird] = step;
        }
        if (diodeStart[diodeThird] != -1 && diodeLight(diodeThird, step, diodeStart[diodeThird], duration)) {
          diodeStart[diodeThird] = -1;
          
          diodeComplete[diodeThird] = 1;
          
          delay(strobe_offset);
          ++diodeState;
          
          step = -1;
        }
      }
  } else {
    uint8_t diodeIndex = (diodeState / 2) % 3;
    
    if (diodeStart[diodeIndex] == -1) {
      diodeStart[diodeIndex] = step;
    }
    if (diodeLight(diodeIndex, step, diodeStart[diodeIndex], duration)) {
      diodeStart[diodeIndex] = -1;
      
      diodeComplete[0] =
      diodeComplete[1] =
      diodeComplete[2] = 0;
      
      diodeIndexNext = diodeIndex + 1;
      
      delay(strobe_offset);
      ++diodeState;
      
      step = -1;
    }
  }
  ++step;
}

static boolean diodeLight(uint8_t ledIdx, unsigned step, unsigned start, unsigned ticksToGo) {
  float span = ((step - start) % ticksToGo) / (float) (ticksToGo - 1);
  
  if (span < 0.25f) {
    analogWriteLite(diodes[ledIdx], 255 * span / 0.25f);
  } else if (span >= 0.5f) { 
    analogWriteLite(diodes[ledIdx], 255 * (1 - span) / 0.5f);

    return span >= 1.0f;
  }
  return false;
}


IRrecv irrecv(2);
static decode_results decodeResults;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(3, INPUT);
  
  pinMode(diodes[0], OUTPUT);
  pinMode(diodes[1], OUTPUT);
  pinMode(diodes[2], OUTPUT);
  
  irrecv.enableIRIn();
  irrecv.blink13(true);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}


#define BUTTON_VOLUME_DN 0xffe01f
#define BUTTON_VOLUME_UP 0xffa857
#define BUTTON_HOLD      -1

static void processRemoteButton(const decode_results &decodeResults) {
  static unsigned long currentCode = -1;
  
  if (decodeResults.decode_type == NEC)
  {
    if (decodeResults.value != BUTTON_HOLD 
      && decodeResults.value != currentCode) {      
      currentCode = decodeResults.value;
    }
    switch (currentCode)
    {
      case BUTTON_VOLUME_DN:
        lightDuration = min(lightDuration + LIGHT_DURATION_DELTA, LIGHT_DURATION_MAX);
        break;
      
      case BUTTON_VOLUME_UP:
        lightDuration = max(lightDuration - LIGHT_DURATION_DELTA, LIGHT_DURATION_MIN);
        break;
    }
  }
}


static volatile uint8_t *tvalue[] = { 0, 0, 0, 0, 0, &OCR0B, &OCR0A, 0, 0, reinterpret_cast<volatile uint8_t*>(&OCR1A), 0, 0, 0, 0 };

static inline void analogWriteLite(uint8_t pin, int value) {
  *tvalue[pin] = (uint8_t) value;
}


static void dream(uint8_t, void (*)(), int);

void loop() {
  static boolean lightsEnabled = false;
  
  unsigned lightness = map(analogRead(0), 0, 144, 1024, 0); // Max voltage here is 0.7 (= Vbe of KT315)
  
  #ifdef DEBUG
  Serial.println(lightness);
  #else
  if (lightness > 512) {
    illuminationReset();
    
    dream(1, 0, LOW);
    
    lightsEnabled = false;
  }
  if (irrecv.decode(&decodeResults)) {
    processRemoteButton(decodeResults);

    irrecv.resume();
  }
  if (!lightsEnabled) {
    lightsEnabled = true;
    
    illuminationInitialize();
  }
  illumination(lightDuration);
  #endif
}


static uint8_t sleepPinCurrent    = -1;
static void (*sleepFuncCurrent)() =  0;
  
static void dream(uint8_t pin, void (*sleepFunc)(), int mode) {
  sleepInterrupt();
  
  cli();
  {
    set_sleep_mode(SLEEP_MODE_STANDBY);
    sleep_bod_disable();
    sleep_enable(); 
  }
  sei();
  
  sleepPinCurrent = pin;
  sleepFuncCurrent = sleepFunc; 
  
  attachInterrupt(pin, sleepInterrupt, mode);
  digitalWrite(13, LOW);
  
  sleep_cpu(); 
  sleep_disable();
  sleep_bod_enable();
}

static void sleepInterrupt() {
  if (sleepPinCurrent != -1) {
    if (sleepFuncCurrent) {
      sleepFuncCurrent();
      
      sleepFuncCurrent = 0;
    }
    detachInterrupt(sleepPinCurrent);
    
    sleepPinCurrent = -1;
  }
}

static void sleep_bod_disable() {
  MCUCR |= _BV(BODS);
}

static void sleep_bod_enable() {
  MCUCR &= ~_BV(BODS);
}

