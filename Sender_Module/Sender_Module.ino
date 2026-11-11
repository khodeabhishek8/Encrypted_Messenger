/* ESP8266 plain-text sender module
 * Open http://ESP_IP/msg in a browser.
 * Type a message and press Send. The exact text is printed on Serial Monitor.
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "sender_ui.h"

#ifndef STASSID
#define STASSID "Airtel__Khode"
#define STAPSK  "Airtel_Kh0de"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

void handleSenderPage() {
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html; charset=utf-8", sender_html);
}

void handleMessage() {
  // For Content-Type: text/plain, ESP8266WebServer stores the raw body as "plain".
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain; charset=utf-8", "No message body received.");
    Serial.println(F("[WEB] Rejected POST: request body was empty."));
    return;
  }

  String message = server.arg("plain");
  if (message.length() == 0) {
    server.send(400, "text/plain; charset=utf-8", "Message is empty.");
    Serial.println(F("[WEB] Rejected POST: empty message."));
    return;
  }
  if (message.length() > 500) {
    server.send(413, "text/plain; charset=utf-8", "Message is too long (maximum 500 bytes).");
    Serial.println(F("[WEB] Rejected POST: message too long."));
    return;
  }

  Serial.println();
  Serial.println(F("========== MESSAGE RECEIVED =========="));
  Serial.println(message); // Print the original plain text exactly as posted.
  Serial.println(F("======================================"));

  server.send(200, "text/plain; charset=utf-8", "Message received by ESP8266.");
}

void handleRoot() {
  server.sendHeader("Location", "/msg", true);
  server.send(302, "text/plain", "Opening sender page...");
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println(F("ESP8266 Plain-Text Sender starting..."));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print(F("Connecting to Wi-Fi"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.println(F("Wi-Fi connected."));
  Serial.print(F("Open sender page: http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/msg"));
  Serial.println(F("Serial Monitor baud rate: 115200"));

  server.on("/", HTTP_GET, handleRoot);
  server.on("/msg", HTTP_GET, handleSenderPage);
  server.on("/msg", HTTP_POST, handleMessage);
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found. Open /msg");
  });
  server.begin();
  Serial.println(F("HTTP server ready."));
}

void loop() {
  server.handleClient();
}
