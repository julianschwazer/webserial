#include <SimpleWebSerial.h>;
SimpleWebSerial WebSerial;

void setup() {
    Serial.begin(57600);
    
    // Defining an event listener with event name and callback function.
    WebSerial.on("event", doSomething);
}

// This is our callback function.
// JSONVar is the type of the parameter and has to be explicitly specified.
void doSomething(JSONVar parameter) {
    Serial.println(parameter);
}

void loop() {
    
}
