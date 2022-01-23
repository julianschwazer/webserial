#include <SimpleWebSerial.h>
SimpleWebSerial WebSerial;

unsigned long counter = 0;
unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(57600);

  WebSerial.on("event", toggleLights);
}


void toggleLights(JSONVar parameter) {
  int value = (int)parameter;
  
  if(value == 1) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }
}


void loop() {

  WebSerial.check(); // checking if there is serial data
 
  /* sending random values to the arduino */
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    counter++;
    int modcount = counter % 4 + 1;
    WebSerial.send("value", modcount); // send
  }
}
