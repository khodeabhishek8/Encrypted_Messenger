/*
  ESP8266 RECEIVER MODULE

  Web page:       http://RECEIVER_IP/receive
  WebSocket:      ws://RECEIVER_IP:81/
  Serial input:   Type a message in Serial Monitor and press Send.

  The webpage is embedded in receiver_ui.h. Keep that header in the
  same Arduino sketch folder as this .ino file.

  For this first test, peerConnected is hardcoded to true. The browser
  receives the initial status snapshot when it connects. Later, call
  setPeerConnected(true/false) only when your real ESP-to-ESP link state
  changes; duplicate status events are suppressed.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include "receiver_ui.h"

// ===== CHANGE THESE WIFI CREDENTIALS =====
const char* WIFI_SSID = "Airtel__Khode";
const char* WIFI_PASSWORD = "Airtel_Kh0de";
// =========================================

ESP8266WebServer httpServer(80);
WebSocketsServer webSocket(81);

const size_t MAX_SERIAL_MESSAGE = 500;
char serialBuffer[MAX_SERIAL_MESSAGE + 1];
size_t serialLength = 0;
bool serialOverflow = false;

// Initial prototype state requested: assume the other ESP is connected.
bool peerConnected = true;

String jsonEscape(const String& input) {
  String escaped;
  escaped.reserve(input.length() + 16);

  const char hex[] = "0123456789abcdef";
  for (size_t i = 0; i < input.length(); i++) {
    const uint8_t c = static_cast<uint8_t>(input[i]);
    switch (c) {
      case '"': escaped += "\\\""; break;
      case '\\': escaped += "\\\\"; break;
      case '\b': escaped += "\\b"; break;
      case '\f': escaped += "\\f"; break;
      case '\n': escaped += "\\n"; break;
      case '\r': escaped += "\\r"; break;
      case '\t': escaped += "\\t"; break;
      default:
        if (c < 0x20) {
          escaped += "\\u00";
          escaped += hex[(c >> 4) & 0x0F];
          escaped += hex[c & 0x0F];
        } else {
          // Bytes >= 0x20 are retained, including UTF-8 multibyte text.
          escaped += static_cast<char>(c);
        }
        break;
    }
  }
  return escaped;
}

String makeStatusPacket() {
  return String("{\"type\":\"status\",\"connected\":") +
         (peerConnected ? "true}" : "false}");
}

String makeMessagePacket(const String& message) {
  String packet;
  packet.reserve(message.length() + 32);
  packet = "{\"type\":\"message\",\"text\":\"";
  packet += jsonEscape(message);
  packet += "\"}";
  return packet;
}

// Call this only when the actual connection to the other ESP changes.
// Repeated calls with the same value do not send another status update.
void setPeerConnected(bool connected) {
  if (peerConnected == connected) {
    return;
  }

  peerConnected = connected;
  // WebSockets library expects a non-const String reference.
  String packet = makeStatusPacket();
  webSocket.broadcastTXT(packet);

  Serial.print(F("[PEER STATUS] "));
  Serial.println(peerConnected ? F("Connected") : F("Disconnected"));
}

void handleRoot() {
  httpServer.sendHeader("Location", "/receive", true);
  httpServer.send(302, "text/plain", "Open /receive");
}

void handleReceivePage() {
  httpServer.sendHeader("Cache-Control", "no-store");
  httpServer.send_P(200, "text/html; charset=utf-8", receiver_html);
}

void onWebSocketEvent(uint8_t clientId, WStype_t type,
                      uint8_t* payload, size_t length) {
  (void)payload;
  (void)length;

  switch (type) {
    case WStype_CONNECTED: {
      IPAddress remoteIP = webSocket.remoteIP(clientId);
      Serial.printf("[WebSocket] Browser connected: %u.%u.%u.%u\n",
                    remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
      // Send one initial state snapshot to this newly connected browser.
      // Keep the String in a named, mutable variable: this library overload
      // takes String& rather than const String& or a temporary String.
      String statusPacket = makeStatusPacket();
      webSocket.sendTXT(clientId, statusPacket);
      break;
    }

    case WStype_DISCONNECTED:
      Serial.printf("[WebSocket] Browser disconnected (client %u)\n", clientId);
      break;

    default:
      break;
  }
}

void broadcastSerialMessage(const String& message) {
  if (message.length() == 0) {
    return;
  }

  Serial.println();
  Serial.println(F("========== SERIAL MESSAGE =========="));
  Serial.println(message);
  Serial.println(F("====================================="));

  // WebSockets library expects a non-const String reference.
  String packet = makeMessagePacket(message);
  webSocket.broadcastTXT(packet);
  Serial.printf("[WebSocket] Message pushed to %u browser(s).\n",
                webSocket.connectedClients());
}

// Serial Monitor input is line-based. Set its line ending to Newline.
void readSerialMonitorInput() {
  while (Serial.available() > 0) {
    const char incoming = static_cast<char>(Serial.read());

    if (incoming == '\r') {
      continue;
    }

    if (incoming == '\n') {
      if (serialOverflow) {
        Serial.println(F("[Serial] Message too long; discarded (500 character limit)."));
      } else if (serialLength > 0) {
        serialBuffer[serialLength] = '\0';
        String message(serialBuffer);
        broadcastSerialMessage(message);
      }

      serialLength = 0;
      serialOverflow = false;
      continue;
    }

    if (serialOverflow) {
      continue;  // Discard until the line ending arrives.
    }

    if (serialLength < MAX_SERIAL_MESSAGE) {
      serialBuffer[serialLength++] = incoming;
    } else {
      serialOverflow = true;
    }
  }
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print(F("Connecting to Wi-Fi"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print('.');
    yield();
  }

  Serial.println();
  Serial.println(F("Wi-Fi connected."));
  Serial.print(F("Receiver page: http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/receive"));
  Serial.print(F("WebSocket: ws://"));
  Serial.print(WiFi.localIP());
  Serial.println(F(":81/"));
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  delay(150);

  Serial.println();
  Serial.println(F("====================================="));
  Serial.println(F("       ESP8266 RECEIVER MODULE       "));
  Serial.println(F("====================================="));

  connectWiFi();

  if (MDNS.begin("esp8266-receiver")) {
    Serial.println(F("mDNS: http://esp8266-receiver.local/receive"));
  }

  httpServer.on("/", HTTP_GET, handleRoot);
  httpServer.on("/receive", HTTP_GET, handleReceivePage);
  httpServer.onNotFound([]() {
    httpServer.send(404, "text/plain", "Not found. Open /receive");
  });

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  httpServer.begin();

  Serial.println(F("HTTP server ready on port 80."));
  Serial.println(F("WebSocket server ready on port 81."));
  Serial.println(F("Peer status initial value: CONNECTED (hardcoded)."));
  Serial.println(F("Type a line in Serial Monitor and press Send."));
  Serial.println(F("Set Serial Monitor line ending to Newline."));
  Serial.println(F("READY."));
}

void loop() {
  httpServer.handleClient();
  webSocket.loop();
  readSerialMonitorInput();
  MDNS.update();
  yield();
}
