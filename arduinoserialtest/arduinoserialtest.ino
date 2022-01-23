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

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    counter++;
    int modcount = counter % 4 + 1;
    WebSerial.send("value", modcount);
  }
  
}
