#include <IRremote.h>
#include <Servo.h>

int RECV_PIN = 7;
int SERVO_PIN = 9;

/**
 * remote controller buttons codes
 */
long BUTTON_ONE = 0xFF30CF;
long BUTTON_TWO = 0xFF18E7;
long BUTTON_THREE = 0xFF7A85;
long PLAY_BUTTON = 0xFFC23D;
long LEFT_BUTTON = 0xFF22DD;
long RIGHT_BUTTON = 0xFF02FD;

/** 
 * IRremote initialization
 */
IRrecv irrecv(RECV_PIN);
decode_results results;

/**
 * creating servo object
 */
Servo servo;

void setup()
{
  servo.attach(SERVO_PIN);
  irrecv.enableIRIn();

  /** 
   * debug
   */
  Serial.begin(9600);
}

void enable(Servo &servo) {
  servo.write(120);
}

void disable(Servo &servo) {
  servo.write(55);
}

void default_position(Servo &servo) {
  servo.write(90);
}

boolean is_in_default_position = false;


void loop() {
  if(!is_in_default_position) {
    default_position(servo);
    is_in_default_position = true;
  }

  /*
   * waiting for button from ir controller
   */
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);

    /*
     * write to servo
     */
    if(results.value == LEFT_BUTTON) {
      disable(servo);
      delay(15);
    }
    if(results.value == RIGHT_BUTTON) {
      enable(servo);
      delay(15);
    }

    if(results.value == PLAY_BUTTON) {
      default_position(servo);
      delay(15);
    }
    
    irrecv.resume(); 
  }
}
