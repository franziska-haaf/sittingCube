#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

uint16_t red = 0xF720Df;
uint16_t redRawData[71] = {9016, 4496,  580, 550,  608, 522,  582, 548,  608, 522,  582, 548,  584, 546,  582, 548,  610, 520,  582, 1654,  608, 1628,  606, 1630,  582, 1654,  584, 546,  584, 1652,  584, 1652,  584, 1652,  582, 548,  582, 548,  580, 1654,  584, 546,  586, 548,  574, 552,  584, 546,  584, 546,  584, 1654,  582, 1654,  584, 546,  608, 1628,  606, 1630,  580, 1654,  582, 1654,  582, 1654,  582, 41022,  9010, 2234,  580};  // NEC F720DF

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

void setup() {
  irsend.begin();
  Serial.begin(115200);
}

void loop() {
  delay(10000);
  Serial.println("a rawData capture from IRrecvDumpV2");
  irsend.sendRaw(redRawData, 71, 38);  // Send a raw data capture at 38kHz.
}
