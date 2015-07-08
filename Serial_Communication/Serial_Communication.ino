#include <AltSoftSerial.h>

AltSoftSerial mySerial;

void setup() {
  Serial.begin(9600);
  
  mySerial.begin(9600);
  mySerial.println("Software serial is ready.");

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

static byte buffer[768];

void loop() {
 while (mySerial.available()) {
  Serial.write(buffer, mySerial.readBytes(buffer, sizeof(buffer)));
 }
}
