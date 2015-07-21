#define PIN_STEP_A 2
#define PIN_STEP_B 3
#define PIN_STEP_C 4
#define PIN_STEP_D 5

void setup() {
  pinMode(PIN_STEP_A, OUTPUT);
  pinMode(PIN_STEP_B, OUTPUT);
  pinMode(PIN_STEP_C, OUTPUT);
  pinMode(PIN_STEP_D, OUTPUT);
}


int rotationVector[][2] = {
  { PIN_STEP_A, -1 }, { PIN_STEP_A, PIN_STEP_B },
  { PIN_STEP_B, -1 }, { PIN_STEP_B, PIN_STEP_C },
  { PIN_STEP_C, -1 }, { PIN_STEP_C, PIN_STEP_D },
  { PIN_STEP_D, -1 }, { PIN_STEP_D, PIN_STEP_A },
};

int index = 0;
int indexPrevious = 0;

void loop() {
  digitalWrite(rotationVector[indexPrevious][0], LOW);
  digitalWrite(rotationVector[index][0], HIGH);

  if (rotationVector[indexPrevious][1] > 0) {
    digitalWrite(rotationVector[index][1], LOW);
  }
  if (rotationVector[index][1] > 0) {
    digitalWrite(rotationVector[index][1], HIGH);
  }
  delayMicroseconds(860);

  indexPrevious = index;
  index = (index + 1) % 8;
}
