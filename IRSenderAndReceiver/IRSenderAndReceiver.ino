#include <Arduino.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

//Pins
#define IR_TRANSMITTER_PIN 4
const uint16_t IR_receiving_pin = 5;

// Colors
uint16_t red = 0xF720Df;
uint16_t rawData[71] = {9052, 4452,  628, 500,  628, 502,  628, 500,  626, 504,  626, 504,  626, 504,  622, 508,  620, 508,  624, 1612,  622, 1612,  628, 1608,  628, 1608,  626, 502,  618, 1620,  620, 1612,  622, 1614,  622, 1614,  616, 514,  562, 1674,  590, 1644,  564, 564,  582, 548,  562, 568,  562, 566,  562, 568,  560, 1674,  560, 568,  560, 568,  560, 1674,  560, 1676,  582, 1654,  558, 1676,  582, 41000,  9032, 2230,  584};  // NEC F7B04F

uint16_t brighterRaw[67] = {8948, 4558,  520, 586,  546, 610,  520, 610,  520, 608,  522, 606,  522, 608,  546, 584,  522, 610,  546, 1688,  522, 1690,  544, 1694,  544, 1690,  546, 608,  520, 1716,  520, 1714,  546, 1690,  522, 586,  542, 610,  520, 586,  544, 610,  518, 610,  520, 610,  518, 610,  522, 608,  522, 1712,  524, 1714,  520, 1690,  544, 1716,  522, 1690,  568, 1670,  542, 1718,  518, 1714,  522};  // NEC F700FF
uint16_t darkerRaw[67] = {8932, 4546,  522, 604,  546, 580,  520, 606,  518, 610,  616, 512,  518, 586,  568, 582,  492, 634,  548, 1686,  516, 1686,  544, 1712,  520, 1712,  518, 606,  522, 1708,  520, 1710,  520, 1710,  520, 1688,  542, 582,  546, 606,  520, 608,  518, 608,  520, 582,  544, 608,  518, 584,  542, 606,  520, 1712,  520, 1690,  540, 1710,  520, 1706,  520, 1714,  520, 1710,  520, 1710,  518};  // NEC F7807F
uint16_t offRaw[67] = {8942, 4556,  518, 634,  520, 582,  522, 610,  520, 608,  518, 608,  518, 586,  544, 612,  518, 610,  522, 1714,  516, 1716,  544, 1688,  520, 1744,  516, 584,  516, 1718,  516, 1716,  546, 1686,  522, 636,  518, 1688,  520, 612,  516, 610,  520, 608,  520, 608,  520, 610,  520, 634,  468, 1768,  490, 610,  518, 1714,  520, 1712,  518, 1718,  542, 1688,  518, 1718,  518, 1716,  516};  // NEC F740BF
uint16_t onRaw[67] = {8976, 4488,  548, 576,  550, 580,  546, 580,  572, 552,  550, 578,  574, 554,  550, 576,  576, 550,  550, 1678,  572, 1654,  548, 1680,  550, 1680,  548, 578,  548, 1680,  562, 1668,  548, 1680,  548, 1682,  572, 1654,  574, 554,  574, 552,  546, 580,  546, 578,  574, 554,  548, 578,  548, 578,  548, 580,  572, 1654,  550, 1682,  570, 1658,  544, 1682,  546, 1682,  544, 1682,  572};  // NEC F7C03F

uint16_t redRaw[67] = {8950, 4562,  520, 610,  518, 612,  520, 610,  520, 610,  546, 582,  522, 610,  522, 608,  522, 608,  522, 1716,  522, 1692,  572, 1688,  522, 1716,  518, 610,  522, 1718,  520, 1714,  550, 1686,  520, 612,  546, 556,  548, 1690,  546, 610,  574, 554,  522, 608,  522, 584,  546, 516,  612, 1716,  522, 1714,  522, 610,  520, 1716,  548, 1662,  572, 1688,  574, 1664,  520, 1716,  522};  // NEC F720DF
uint16_t red2Raw[67] = {8982, 4530,  552, 580,  574, 556,  574, 556,  630, 498,  576, 554,  552, 578,  550, 580,  576, 554,  550, 1686,  550, 1686,  574, 1662,  576, 1658,  548, 584,  548, 1686,  550, 1688,  550, 1686,  550, 580,  548, 584,  548, 580,  576, 1662,  548, 580,  550, 580,  574, 558,  548, 582,  546, 1688,  548, 1690,  546, 1690,  546, 582,  574, 1662,  548, 1688,  546, 1688,  546, 1712,  524};  // NEC F710EF
uint16_t red3Raw[67] = {8966, 4528,  520, 612,  514, 636,  494, 608,  520, 608,  522, 610,  490, 612,  544, 608,  520, 610,  518, 1714,  518, 1714,  516, 1714,  546, 1688,  518, 638,  548, 1658,  544, 1688,  548, 1684,  520, 608,  520, 638,  464, 1742,  518, 1714,  546, 582,  544, 584,  520, 610,  518, 608,  522, 1688,  570, 1688,  522, 606,  518, 612,  516, 1716,  546, 1658,  546, 1710,  522, 1714,  520};  // NEC F730CF
uint16_t red4Raw[67] = {8976, 4532,  546, 582,  572, 558,  546, 582,  572, 558,  574, 558,  572, 558,  546, 604,  524, 582,  572, 1686,  548, 1664,  546, 1712,  524, 1690,  546, 606,  520, 1714,  548, 1686,  548, 1688,  546, 584,  546, 584,  520, 606,  538, 594,  546, 1690,  520, 608,  546, 584,  520, 608,  520, 1716,  544, 1692,  520, 1716,  520, 1716,  522, 608,  546, 1690,  518, 1716,  546, 1690,  522};  // NEC F708F7

uint16_t greenRaw[67] = {9008, 4502,  580, 550,  578, 550,  580, 550,  578, 554,  578, 554,  578, 552,  578, 550,  578, 552,  554, 1684,  578, 1658,  578, 1658,  578, 1660,  578, 552,  578, 1660,  578, 1660,  578, 1660,  578, 1660,  576, 556,  576, 1660,  576, 554,  576, 554,  576, 554,  576, 554,  574, 556,  574, 556,  574, 1662,  574, 556,  576, 1662,  574, 1662,  574, 1664,  574, 1664,  572, 1664,  574};  // NEC F7A05F


uint16_t blueRaw[67] = {8950, 4564,  520, 610,  522, 638,  494, 584,  542, 612,  518, 612,  548, 580,  548, 582,  522, 608,  522, 1714,  520, 1718,  522, 1690,  546, 1690,  544, 612,  520, 1694,  544, 1672,  564, 1718,  520, 586,  546, 1692,  544, 1716,  548, 582,  520, 610,  520, 612,  520, 610,  518, 610,  518, 1694,  570, 582,  520, 610,  520, 1716,  518, 1698,  542, 1718,  522, 1716,  518, 1694,  546};  // NEC F7609F

//Values
IRsend irsend(IR_TRANSMITTER_PIN);

const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint16_t kMinUnknownSize = 12;
const uint16_t kTimeout = 15;


// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(IR_receiving_pin, kCaptureBufferSize, kTimeout, true);

decode_results results;  // Somewhere to store the results

void setup() {
  irsend.begin();
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial)
    delay(50);
  Serial.println();
  Serial.print("IRrecvDumpV2 is now running and waiting for IR input on Pin ");
  Serial.println(IR_receiving_pin);

#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif                  // DECODE_HASH
  irrecv.enableIRIn();  // Start the receiver
}


int waitTime = 20000; //20 seconds
unsigned long timeNow = 0;
void sendIRSignal() {
  if ((unsigned long)(millis() - timeNow) > waitTime) {
    timeNow = millis();
    Serial.println("Send Data");
    //irsend.sendNEC(0xF710EF, 32);
    irsend.sendRaw(rawData, 71, 38);  // Send a raw data capture at 38kHz.
  }
}


void loop() {
  sendIRSignal();

  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    uint32_t now = millis();
    Serial.printf("GOT SOMETHING\n");
    Serial.printf("Timestamp : %06u.%03u\n", now / 1000, now % 1000);
    if (results.overflow)
      Serial.printf(
        "WARNING: IR code is too big for buffer (>= %d). "
        "This result shouldn't be trusted until this is resolved. "
        "Edit & increase kCaptureBufferSize.\n",
        kCaptureBufferSize);
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&results));
    yield();  // Feed the WDT as the text output can take a while to print.

    // Display the library version the message was captured with.
    Serial.print("Library   : v");
    Serial.println(_IRREMOTEESP8266_VERSION_);
    Serial.println();

    // Output RAW timing info of the result.
    Serial.println(resultToTimingInfo(&results));
    yield();  // Feed the WDT (again)

    // Output the results as source code
    Serial.println(resultToSourceCode(&results));
    Serial.println("");  // Blank line between entries
    yield();             // Feed the WDT (again)
  }
}
