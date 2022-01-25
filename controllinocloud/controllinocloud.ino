#include <SPI.h>
#include <Controllino.h>

#include <SimpleWebSerial.h>
SimpleWebSerial WebSerial;

int buttonOneState = digitalRead(CONTROLLINO_A1);
int buttonTwoState = digitalRead(CONTROLLINO_A2);
int buttonThreeState = digitalRead(CONTROLLINO_A3);
int buttonFourState = digitalRead(CONTROLLINO_A4);

int oldButtonOneState = digitalRead(CONTROLLINO_A1);
int oldButtonTwoState = digitalRead(CONTROLLINO_A2);
int oldButtonThreeState = digitalRead(CONTROLLINO_A3);
int oldButtonFourState = digitalRead(CONTROLLINO_A4);

unsigned long currentMillis = millis();
unsigned long rainTimer = millis();

boolean idleState = true;
boolean rainState = false;

const byte pwmLED = CONTROLLINO_D8;

int rainTime = 5000;


// Define directions for LED fade
#define UP 0
#define DOWN 1

// Constants for min and max PWM
const int minPWM = 0;
const int maxPWM = 255;

// State Variable for Fade Direction
byte fadeDirection = UP;

// Global Fade Value
// but be bigger than byte and signed, for rollover
int fadeValue = 0;

// How smooth to fade?
byte fadeIncrement = 1;

// millis() timing Variable, just for fading
unsigned long previousFadeMillis;

// How fast to increment?
int fadeInterval = 10;



void setup() {

  pinMode(CONTROLLINO_A1, INPUT);
  pinMode(CONTROLLINO_A2, INPUT);
  pinMode(CONTROLLINO_A3, INPUT);
  pinMode(CONTROLLINO_A4, INPUT);

  analogWrite(pwmLED, fadeValue);

  pinMode(CONTROLLINO_R5, OUTPUT);
  digitalWrite(CONTROLLINO_R5, LOW);

  Serial.begin(57600);

  WebSerial.on("event", toggleLights);

}



void loop() {

  WebSerial.check(); // checking if there is serial data

  ReadButtons();
  // ReadSerial();
  CheckButtonOne();
  CheckButtonTwo();
  CheckButtonThree();
  CheckButtonFour();

  currentMillis = millis();

  if (idleState == true) {
    cloudFade(currentMillis);
  }

  if ((currentMillis - previousFadeMillis >= rainTime) && (rainState == true)) {
    digitalWrite(CONTROLLINO_R5, LOW);
    rainState = false;
    idleState = true;
  }
  if ((currentMillis - previousFadeMillis <= rainTime) && (rainState == true)) {
    digitalWrite(CONTROLLINO_R5, HIGH);
  }

}



void CheckButtonOne() {
  if (buttonOneState != oldButtonOneState) {

    WebSerial.send("value", 0);
    rainState = true;
    idleState = false;
    rainTimer = millis();
    digitalWrite(CONTROLLINO_R5, HIGH);
  }
  oldButtonOneState = buttonOneState;
}

void CheckButtonTwo() {
  if (buttonTwoState != oldButtonTwoState) {

    WebSerial.send("value", 1);
    rainState = true;
    idleState = false;
    rainTimer = millis();
    digitalWrite(CONTROLLINO_R5, HIGH);
  }
  oldButtonTwoState = buttonTwoState;
}

void CheckButtonThree() {
  if (buttonThreeState != oldButtonThreeState) {

    WebSerial.send("value", 2);
    rainState = true;
    idleState = false;
    rainTimer = millis();
    digitalWrite(CONTROLLINO_R5, HIGH);
  }
  oldButtonThreeState = buttonThreeState;
}

void CheckButtonFour() {
  if (buttonFourState != oldButtonFourState) {

    WebSerial.send("value", 3);
    rainState = true;
    idleState = false;
    rainTimer = millis();
    digitalWrite(CONTROLLINO_R5, HIGH);
  }
  oldButtonFourState = buttonFourState;
}

void ReadButtons() {
  buttonOneState = digitalRead(CONTROLLINO_A1);
  buttonTwoState = digitalRead(CONTROLLINO_A2);
  buttonThreeState = digitalRead(CONTROLLINO_A3);
  buttonFourState = digitalRead(CONTROLLINO_A4);
}

void toggleLights(JSONVar parameter) {
  int value = (int)parameter;

  // Serial.println(value);

  if (value == 1) {
    idleState = true;
    rainState = false;
  }
  else if (value == 0) {
    // idleState = true;
    // rainState = false;
  }
}

/*
  void ReadSerial() {
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0) {
      // idleState = true;
      // rainState = false;

    } else if (inByte == 1) {
      idleState = true;
      rainState = false;
    }
  }
  delay(5);
  }
*/

void cloudFade(unsigned long currentMillis) {
  // is it time to update yet?
  // if not, nothing happens
  if (currentMillis - previousFadeMillis >= fadeInterval) {
    // yup, it's time!
    if (fadeDirection == UP) {
      fadeValue = fadeValue + fadeIncrement;
      if (fadeValue >= maxPWM) {
        // At max, limit and change direction
        fadeValue = maxPWM;
        fadeDirection = DOWN;
      }
    } else {
      //if we aren't going up, we're going down
      fadeValue = fadeValue - fadeIncrement;
      if (fadeValue <= minPWM) {
        // At min, limit and change direction
        fadeValue = minPWM;
        fadeDirection = UP;
      }
    }
    // Only need to update when it changes
    analogWrite(pwmLED, fadeValue);

    // reset millis for the next iteration (fade timer only)
    previousFadeMillis = currentMillis;
  }
}
