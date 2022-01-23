
#include <SimpleWebSerial.h>
SimpleWebSerial WebSerial;

unsigned long counter = 0;
unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(57600);
}

void loop() {

  ReadSerial();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    counter++;
    int modcount = counter % 4 + 1;
    WebSerial.send("value", modcount);
  }
}



void ReadSerial() {
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0) {
      digitalWrite(13, HIGH);
    } else if (inByte == 1) {
      digitalWrite(13, LOW);
    }
  }
  delay(5);
}
