#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <IRremote.hpp>

// -------- WIFI --------
const char* ssid = "ESP_CONTROL";
const char* password = "12345678";
WiFiServer server(80);

// -------- SERIAL --------
SoftwareSerial esp( , D3); // RX, TX

// -------- IR --------
#define IR_RECEIVE_PIN D5   // IMPORTANT (avoid conflict)

// -------- GLOBAL --------
String DATA = "";

// ============================================
void setup() {
  Serial.begin(9600);
  esp.begin(9600);

  // IR start
  IrReceiver.begin(IR_RECEIVE_PIN);

  // WiFi start
  WiFi.softAP(ssid, password);
  server.begin();

  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  Serial.println("System Ready...");
}
// ============================================

void loop() {
  comms();       // serial control
  webpage();     // wifi control
  irControl();   // IR control
}

// ============================================
// -------- SERIAL CONTROL --------
void comms() {
  if (Serial.available()) {
    String received = Serial.readStringUntil('\n');
    received.trim();

    if (received == "FS" || received == "Front") DATA = "front";
    else if (received == "BS"|| received == "Back") DATA = "back";
    else if (received == "LS"|| received == "Left") DATA = "left";
    else if (received == "RS"|| received == "Right") DATA = "right";
    else if (received == "Stop"|| received == "stop") DATA = "stop";
    else DATA = received;

    Serial.print("Serial: ");
    Serial.println(DATA);

    esp.println(DATA);
    delay(150);
  }
}

// ============================================
// -------- IR CONTROL --------
void irControl() {
  if (IrReceiver.decode()) {

    unsigned long value = IrReceiver.decodedIRData.decodedRawData;

    // ignore repeat/noise
    if (value != 0 && value != 0xFFFFFFFF) {

      Serial.print("IR: ");
      Serial.println(value, HEX);

      if (value == 0xF50A7F80) DATA = "front";
      else if (value == 0xE41B7F80) DATA = "back";
      else if (value == 0xE01F7F80) DATA = "left";
      else if (value == 0xF30C7F80) DATA = "right";
      else if (value == 0xF8077F80) DATA = "stop";
      else DATA= "not a car data";

      if (DATA != "") {
        Serial.print("IR CMD: ");
        Serial.println(DATA);
        esp.println(DATA);
      }
    }

    IrReceiver.resume();
  }
}

// ============================================
// -------- WIFI WEB CONTROL --------
void webpage() {

  WiFiClient client = server.available();

  if (client) {

    String request = client.readStringUntil('\r');
    client.flush();

    String command = "";

    if (request.indexOf("/forward") != -1) command = "front";
    else if (request.indexOf("/back") != -1) command = "back";
    else if (request.indexOf("/left") != -1) command = "left";
    else if (request.indexOf("/right") != -1) command = "right";
    else if (request.indexOf("/stop") != -1) command = "stop";

    if (command != "") {
      DATA = command;
      Serial.print("WiFi: ");
      Serial.println(DATA);
      esp.println(DATA);
    }

    // -------- HTML UI --------
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");

    client.println("<!DOCTYPE html><html><head>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");

    client.println("<style>");
    client.println("body { margin:0; display:flex; justify-content:center; align-items:center; height:100vh; background:black; color:white; font-family:sans-serif;}");
    client.println(".dpad { display:grid; grid-template-columns: 90px 90px 90px; gap:20px;}");
    client.println("button { width:90px; height:90px; font-size:30px; background:purple; color:white; border:none; border-radius:20px;}");
    client.println(".empty { visibility:hidden; }");
    client.println("</style></head><body>");

    client.println("<div class='dpad'>");

    client.println("<div class='empty'></div>");
    client.println("<a href='/forward'><button>F</button></a>");
    client.println("<div class='empty'></div>");

    client.println("<a href='/left'><button>L</button></a>");
    client.println("<a href='/stop'><button>S</button></a>");
    client.println("<a href='/right'><button>R</button></a>");

    client.println("<div class='empty'></div>");
    client.println("<a href='/back'><button>B</button></a>");
    client.println("<div class='empty'></div>");

    client.println("</div>");
    client.println("</body></html>");
  }
}