#include <Arduino.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

// ==================== start of TUNEABLE PARAMETERS ====================
// An IR detector/demodulator is connected to GPIO pin 14
// e.g. D5 on a NodeMCU board.
const uint16_t kRecvPin = 5;
uint16_t red = 0xF720Df;
//uint16_t rawData[71] = {9082, 4430,  654, 480,  650, 480,  652, 478,  652, 478,  652, 478,  652, 478,  652, 478,  652, 478,  652, 1584,  654, 1584,  654, 1584,  652, 1584,  652, 478,  652, 1584,  652, 1584,  654, 1584,  652, 478,  652, 478,  652, 478,  652, 1584,  652, 478,  624, 506,  626, 502,  628, 502,  628, 1608,  628, 1608,  628, 1608,  628, 502,  628, 1608,  628, 1608,  630, 1608,  628, 1608,  628, 41010,  9052, 2188,  604};  // NEC F710EF//Protocol Protocol  : NEC
uint16_t rawData[71] = {9052, 4452,  628, 500,  628, 502,  628, 500,  626, 504,  626, 504,  626, 504,  622, 508,  620, 508,  624, 1612,  622, 1612,  628, 1608,  628, 1608,  626, 502,  618, 1620,  620, 1612,  622, 1614,  622, 1614,  616, 514,  562, 1674,  590, 1644,  564, 564,  582, 548,  562, 568,  562, 566,  562, 568,  560, 1674,  560, 568,  560, 568,  560, 1674,  560, 1676,  582, 1654,  558, 1676,  582, 41000,  9032, 2230,  584};  // NEC F7B04F

#define IR_LED 4
IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

const uint32_t kBaudRate = 115200;

// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
const uint16_t kCaptureBufferSize = 1024;

// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
// This parameter is an interesting trade-off. The longer the timeout, the more
// complex a message it can capture. e.g. Some device protocols will send
// multiple message packets in quick succession, like Air Conditioner remotes.
// Air Coniditioner protocols often have a considerable gap (20-40+ms) between
// packets.
// The downside of a large timeout value is a lot of less complex protocols
// send multiple messages when the remote's button is held down. The gap between
// them is often also around 20+ms. This can result in the raw data be 2-3+
// times larger than needed as it has captured 2-3+ messages in a single
// capture. Setting a low timeout value can resolve this.
// So, choosing the best kTimeout value for your use particular case is
// quite nuanced. Good luck and happy hunting.
// NOTE: Don't exceed kMaxTimeoutMs. Typically 130ms.
#if DECODE_AC
// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t kTimeout = 50;
#else   // DECODE_AC
// Suits most messages, while not swallowing many repeats.
const uint8_t kTimeout = 15;
#endif  // DECODE_AC

// Set the smallest sized "UNKNOWN" message packets we actually care about.
// This value helps reduce the false-positive detection rate of IR background
// noise as real messages. The chances of background IR noise getting detected
// as a message increases with the length of the kTimeout value. (See above)
// The downside of setting this message too large is you can miss some valid
// short messages for protocols that this library doesn't yet decode.
//
// Set higher if you get lots of random short UNKNOWN messages when nothing
// should be sending a message.
// Set lower if you are sure your setup is working, but it doesn't see messages
// from your device. (e.g. Other IR remotes work.)
// NOTE: Set this value very high to effectively turn off UNKNOWN detection.
const uint16_t kMinUnknownSize = 12;
// ==================== end of TUNEABLE PARAMETERS ====================

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);

decode_results results;  // Somewhere to store the results

void setup() {
  irsend.begin();
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDumpV2 is now running and waiting for IR input on Pin ");
  Serial.println(kRecvPin);

#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif                  // DECODE_HASH
  irrecv.enableIRIn();  // Start the receiver
}


int waitTime = 30000; //30 seconds
unsigned long timeNow = 0;
void sendIRSignal() {
  if ((unsigned long)(millis() - timeNow) > waitTime) {
    timeNow = millis();
    Serial.println("Send NEC");
    //irsend.sendNEC(0xF710EF, 32);
    irsend.sendRaw(rawData, 71, 38);  // Send a raw data capture at 38kHz.
  }
}

// The repeating section of the code
//
void loop() {
  sendIRSignal();

  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    // Display a crude timestamp.
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
