#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "wifiAccessData.h"
#include "webpage-snippets.h"

// Load Wi-Fi library
#include <ESP8266WiFi.h>

String buttoState = "off";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(74880);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

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
            if (header.indexOf("GET /red/add") >= 0) {
              Serial.println("Red");
              //todo: set color
            } else if (header.indexOf("GET /blue/add") >= 0) {
              Serial.println("Blue");
              //todo: set color
            } else if (header.indexOf("GET /green/add") >= 0) {
              Serial.println("Green");
              //todo: set color
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
            client.print(resetButtonWebpageSnippet);
            client.print("</body></html>");

            // The HTTP response ends with another blank line
            client.print();
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
