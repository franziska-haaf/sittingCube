#include <Arduino.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

//Pins
#define IR_TRANSMITTER_PIN 4

// Colors
uint16_t red = 0xF720Df;
uint16_t rawData[71] = {9052, 4452,  628, 500,  628, 502,  628, 500,  626, 504,  626, 504,  626, 504,  622, 508,  620, 508,  624, 1612,  622, 1612,  628, 1608,  628, 1608,  626, 502,  618, 1620,  620, 1612,  622, 1614,  622, 1614,  616, 514,  562, 1674,  590, 1644,  564, 564,  582, 548,  562, 568,  562, 566,  562, 568,  560, 1674,  560, 568,  560, 568,  560, 1674,  560, 1676,  582, 1654,  558, 1676,  582, 41000,  9032, 2230,  584};  // NEC F7B04F

uint16_t brighterRaw[67] = {8948, 4558,  520, 586,  546, 610,  520, 610,  520, 608,  522, 606,  522, 608,  546, 584,  522, 610,  546, 1688,  522, 1690,  544, 1694,  544, 1690,  546, 608,  520, 1716,  520, 1714,  546, 1690,  522, 586,  542, 610,  520, 586,  544, 610,  518, 610,  520, 610,  518, 610,  522, 608,  522, 1712,  524, 1714,  520, 1690,  544, 1716,  522, 1690,  568, 1670,  542, 1718,  518, 1714,  522};  // NEC F700FF
uint16_t darkerRaw[67] = {8932, 4546,  522, 604,  546, 580,  520, 606,  518, 610,  616, 512,  518, 586,  568, 582,  492, 634,  548, 1686,  516, 1686,  544, 1712,  520, 1712,  518, 606,  522, 1708,  520, 1710,  520, 1710,  520, 1688,  542, 582,  546, 606,  520, 608,  518, 608,  520, 582,  544, 608,  518, 584,  542, 606,  520, 1712,  520, 1690,  540, 1710,  520, 1706,  520, 1714,  520, 1710,  520, 1710,  518};  // NEC F7807F
uint16_t offRaw[67] = {8942, 4556,  518, 634,  520, 582,  522, 610,  520, 608,  518, 608,  518, 586,  544, 612,  518, 610,  522, 1714,  516, 1716,  544, 1688,  520, 1744,  516, 584,  516, 1718,  516, 1716,  546, 1686,  522, 636,  518, 1688,  520, 612,  516, 610,  520, 608,  520, 608,  520, 610,  520, 634,  468, 1768,  490, 610,  518, 1714,  520, 1712,  518, 1718,  542, 1688,  518, 1718,  518, 1716,  516};  // NEC F740BF
uint16_t onRaw[67] = {8976, 4488,  548, 576,  550, 580,  546, 580,  572, 552,  550, 578,  574, 554,  550, 576,  576, 550,  550, 1678,  572, 1654,  548, 1680,  550, 1680,  548, 578,  548, 1680,  562, 1668,  548, 1680,  548, 1682,  572, 1654,  574, 554,  574, 552,  546, 580,  546, 578,  574, 554,  548, 578,  548, 578,  548, 580,  572, 1654,  550, 1682,  570, 1658,  544, 1682,  546, 1682,  544, 1682,  572};  // NEC F7C03F

//#ffffff   red + blue + green
uint16_t whiteRaw[67] = {9006, 4494,  588, 542,  586, 544,  610, 520,  584, 546,  584, 544,  584, 546,  584, 546,  584, 546,  584, 1650,  586, 1650,  584, 1650,  586, 1648,  584, 548,  584, 1650,  584, 1652,  582, 1654,  582, 1652,  584, 1652,  584, 1652,  582, 546,  582, 546,  582, 548,  582, 548,  582, 546,  582, 548,  582, 548,  582, 548,  582, 1652,  582, 1652,  582, 1652,  582, 1654,  586, 1650,  582};  // NEC F7E01F

//#ff0000   red
uint16_t redRaw[67] = {8950, 4562,  520, 610,  518, 612,  520, 610,  520, 610,  546, 582,  522, 610,  522, 608,  522, 608,  522, 1716,  522, 1692,  572, 1688,  522, 1716,  518, 610,  522, 1718,  520, 1714,  550, 1686,  520, 612,  546, 556,  548, 1690,  546, 610,  574, 554,  522, 608,  522, 584,  546, 516,  612, 1716,  522, 1714,  522, 610,  520, 1716,  548, 1662,  572, 1688,  574, 1664,  520, 1716,  522};  // NEC F720DF
//#fca903   red + red
uint16_t orangeRaw[67] = {8982, 4530,  552, 580,  574, 556,  574, 556,  630, 498,  576, 554,  552, 578,  550, 580,  576, 554,  550, 1686,  550, 1686,  574, 1662,  576, 1658,  548, 584,  548, 1686,  550, 1688,  550, 1686,  550, 580,  548, 584,  548, 580,  576, 1662,  548, 580,  550, 580,  574, 558,  548, 582,  546, 1688,  548, 1690,  546, 1690,  546, 582,  574, 1662,  548, 1688,  546, 1688,  546, 1712,  524};  // NEC F710EF
//#ffcf54   red + red + green
uint16_t lightOrangeRaw[67] = {8966, 4528,  520, 612,  514, 636,  494, 608,  520, 608,  522, 610,  490, 612,  544, 608,  520, 610,  518, 1714,  518, 1714,  516, 1714,  546, 1688,  518, 638,  548, 1658,  544, 1688,  548, 1684,  520, 608,  520, 638,  464, 1742,  518, 1714,  546, 582,  544, 584,  520, 610,  518, 608,  522, 1688,  570, 1688,  522, 606,  518, 612,  516, 1716,  546, 1658,  546, 1710,  522, 1714,  520};  // NEC F730CF
//#fff833   red + green + green
uint16_t citrusRaw[67] = {8976, 4532,  546, 582,  572, 558,  546, 582,  572, 558,  574, 558,  572, 558,  546, 604,  524, 582,  572, 1686,  548, 1664,  546, 1712,  524, 1690,  546, 606,  520, 1714,  548, 1686,  548, 1688,  546, 584,  546, 584,  520, 606,  538, 594,  546, 1690,  520, 608,  546, 584,  520, 608,  520, 1716,  544, 1692,  520, 1716,  520, 1716,  522, 608,  546, 1690,  518, 1716,  546, 1690,  522};  // NEC F708F7
//#f8ff33   green + red
uint16_t yellowRaw[67] = {9072, 4428,  650, 484,  646, 484,  646, 508,  622, 480,  648, 482,  648, 480,  648, 480,  648, 508,  622, 1586,  648, 1586,  650, 1584,  650, 1586,  648, 480,  648, 1584,  650, 1586,  650, 1584,  650, 480,  650, 480,  650, 1584,  650, 480,  650, 1584,  650, 480,  648, 476,  652, 480,  650, 1584,  650, 1584,  650, 480,  650, 1582,  650, 478,  650, 1584,  650, 1582,  652, 1582,  652};  // NEC F728D7

//#00ff00   green
uint16_t greenRaw[67] = {9008, 4502,  580, 550,  578, 550,  580, 550,  578, 554,  578, 554,  578, 552,  578, 550,  578, 552,  554, 1684,  578, 1658,  578, 1658,  578, 1660,  578, 552,  578, 1660,  578, 1660,  578, 1660,  578, 1660,  576, 556,  576, 1660,  576, 554,  576, 554,  576, 554,  576, 554,  574, 556,  574, 556,  574, 1662,  574, 556,  576, 1662,  574, 1662,  574, 1664,  574, 1664,  572, 1664,  574};  // NEC F7A05F
//#00ffdd    green + blue
uint16_t turquoiseRaw[67] = {9006, 4494,  586, 542,  586, 542,  584, 544,  586, 544,  586, 544,  584, 544,  582, 546,  584, 544,  584, 1650,  586, 1650,  586, 1648,  584, 1650,  586, 544,  584, 1650,  584, 1650,  584, 1650,  584, 1650,  584, 544,  584, 546,  582, 1652,  584, 546,  582, 548,  582, 546,  584, 546,  584, 546,  580, 1652,  582, 1652,  586, 544,  582, 1652,  582, 1652,  582, 1654,  582, 1652,  584};  // NEC F7906F
//#00f2ff    green + green + blue
uint16_t lightBlueRaw[67] = {9072, 4424,  652, 506,  622, 480,  650, 506,  622, 480,  650, 506,  622, 480,  648, 508,  622, 506,  622, 1584,  650, 1584,  652, 1584,  650, 1584,  650, 506,  622, 1582,  652, 1584,  650, 1586,  650, 1584,  650, 480,  648, 1584,  650, 1584,  650, 482,  648, 482,  646, 480,  650, 478,  650, 480,  648, 1584,  650, 480,  650, 480,  648, 1584,  650, 1584,  650, 1584,  650, 1584,  650};  // NEC F7B04F
//#00b7ff   blue + blue
uint16_t middleBlueRaw[67] = {9076, 4428,  650, 508,  622, 508,  622, 480,  648, 482,  648, 508,  622, 482,  648, 480,  648, 508,  622, 1586,  648, 1586,  650, 1584,  650, 1584,  652, 482,  646, 1584,  652, 1584,  650, 1586,  650, 1584,  652, 480,  648, 480,  648, 480,  650, 1586,  650, 480,  650, 480,  650, 480,  650, 480,  650, 1584,  652, 1584,  652, 1582,  652, 478,  652, 1584,  650, 1586,  650, 1584,  650};  // NEC F78877
//#1cacff   blue + blue + blue
uint16_t middleBlue2Raw[67] = {9036, 4470,  610, 520,  582, 548,  584, 546,  606, 522,  580, 548,  580, 550,  582, 548,  582, 548,  608, 1628,  582, 1654,  584, 1650,  582, 1654,  580, 548,  584, 1652,  580, 1654,  586, 1650,  582, 1654,  580, 548,  580, 1654,  582, 548,  582, 1654,  582, 548,  586, 544,  582, 546,  580, 550,  584, 1650,  584, 546,  584, 1650,  582, 548,  584, 1652,  582, 1652,  584, 1652,  610};  // NEC F7A857

//#0000ff   blue
uint16_t blueRaw[67] = {8950, 4564,  520, 610,  522, 638,  494, 584,  542, 612,  518, 612,  548, 580,  548, 582,  522, 608,  522, 1714,  520, 1718,  522, 1690,  546, 1690,  544, 612,  520, 1694,  544, 1672,  564, 1718,  520, 586,  546, 1692,  544, 1716,  548, 582,  520, 610,  520, 612,  520, 610,  518, 610,  518, 1694,  570, 582,  520, 610,  520, 1716,  518, 1698,  542, 1718,  522, 1716,  518, 1694,  546};  // NEC F7609F
//#4733ff   blue + blue + green
uint16_t slightlyPurpleBlueRaw[67] = {9006, 4500,  554, 574,  578, 552,  580, 550,  580, 550,  554, 576,  580, 548,  584, 546,  578, 552,  580, 1654,  578, 1658,  576, 1658,  554, 1682,  578, 550,  580, 1656,  580, 1656,  576, 1658,  578, 552,  552, 1684,  578, 552,  576, 1658,  576, 556,  574, 554,  574, 554,  574, 554,  550, 1686,  546, 582,  550, 1686,  548, 582,  550, 1684,  548, 1688,  546, 1712,  524, 1686,  548};  // NEC F750AF
//#ae52ff   blue + blue + red
uint16_t lilacRaw[67] = {9014, 4492,  588, 542,  586, 546,  584, 544,  586, 544,  586, 544,  584, 544,  584, 546,  584, 546,  584, 1652,  584, 1652,  586, 1650,  608, 1630,  582, 548,  584, 1652,  584, 1654,  584, 1652,  584, 546,  586, 1650,  584, 1652,  588, 1648,  584, 546,  582, 548,  582, 546,  582, 546,  582, 1652,  582, 548,  582, 548,  582, 546,  584, 1652,  584, 1652,  582, 1654,  582, 1652,  582};  // NEC F7708F
//#ab36ff   blue + red + red
uint16_t purpleRaw[67] = {8962, 4528,  544, 580,  548, 604,  522, 584,  542, 584,  544, 606,  548, 558,  544, 586,  544, 582,  544, 1712,  548, 1684,  522, 1710,  522, 1710,  522, 602,  526, 1688,  544, 1712,  548, 1684,  546, 584,  522, 1688,  544, 584,  546, 584,  544, 1686,  546, 582,  544, 606,  546, 582,  520, 1690,  542, 608,  544, 1686,  524, 1710,  522, 582,  546, 1710,  522, 1686,  546, 1686,  546};  // NEC F748B7
//#d685ff   blue + red
uint16_t lightPurpleRaw[67] = {9012, 4494,  582, 546,  608, 522,  610, 518,  580, 548,  582, 548,  610, 518,  584, 544,  582, 546,  580, 1654,  578, 1656,  580, 1654,  582, 1652,  584, 546,  584, 1650,  582, 1652,  582, 1652,  584, 544,  582, 1654,  582, 1652,  582, 546,  582, 1652,  580, 548,  580, 548,  580, 548,  580, 1654,  580, 548,  580, 548,  580, 1654,  578, 550,  578, 1654,  580, 1656,  578, 1654,  578};  // NEC F76897

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
