#include <Arduino.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include "infraredCodes.h"

//Pins
#define IR_TRANSMITTER_PIN 4

//Values
IRsend irsend(IR_TRANSMITTER_PIN);

const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint16_t kMinUnknownSize = 12;
const uint16_t kTimeout = 15;

void setup() {
  irsend.begin();
  Serial.begin(kBaudRate);
  while (!Serial)
    delay(50);
  Serial.println();
}

int waitTime = 10000; //10 seconds
unsigned long timeNow = 0;
int colorCounter = 0;
void sendIRSignal() {
  if ((unsigned long)(millis() - timeNow) > waitTime) {
    timeNow = millis();
    Serial.println("Send Data");

    if (colorCounter == 2) {
      colorCounter = 0;
    }
    if (colorCounter == 0) {
      irsend.sendRaw(greenRaw, 71, 38);  // Send a raw data capture at 38kHz.
    }
    if (colorCounter == 1) {
      irsend.sendRaw(redRaw, 71, 38);  // Send a raw data capture at 38kHz.
    }
    colorCounter++;
  }
}


void loop() {
  sendIRSignal();
}
