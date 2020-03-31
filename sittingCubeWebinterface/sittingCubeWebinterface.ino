#include <Arduino.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include "infraredCodes.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "wifiAccessData.h"
#include "webpage-snippets.h"

//Pins
#define IR_TRANSMITTER_PIN 4

//Values
IRsend irsend(IR_TRANSMITTER_PIN);

const uint32_t kBaudRate = 115200;

WiFiServer server(80); //port 80
// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  irsend.begin();
  
  Serial.begin(kBaudRate);
  while (!Serial)
    delay(50);
  Serial.println();

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {  
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //CHANGE THE COLOR -----------------------------------------
            if (header.indexOf("GET /red") >= 0) {
              Serial.println("Red");
              //irsend.sendRaw(redRaw, 71, 38);
              sendIRSignal();
            }
            else if (header.indexOf("GET /blue") >= 0) {
              Serial.println("Blue");
              irsend.sendRaw(blueRaw, 71, 38);
            }
            else if (header.indexOf("GET /green") >= 0) {
              Serial.println("Green");
              irsend.sendRaw(greenRaw, 71, 38);
            }
            else if (header.indexOf("GET /admin/on") >= 0) {
              Serial.println("On");
              irsend.sendRaw(onRaw, 71, 38);
            }
            else if (header.indexOf("GET /admin/off") >= 0) {
              Serial.println("Off");
              irsend.sendRaw(offRaw, 71, 38);
            }


            //DISPLAY PAGE -----------------------------------------
            client.print("<!DOCTYPE html><html>");
            client.print("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.print("<link rel=\"icon\" href=\"data:,\">");
            client.print(styleWebpageSnippet);
            client.print("</head>");

            client.println("<body> <div id=\"bottleWrapper\">");
            client.print(redBottleWebpageSnippet);
            client.print(blueBottleWebpageSnippet);
            client.print(greenBottleWebpageSnippet);
            client.print("</div>");
            client.print(resetButtonWebpageSnippet);
            client.print("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

int colorCounter = 0;
void sendIRSignal() {
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
