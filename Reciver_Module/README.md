# ESP8266 Receiver Module — Serial to WebSocket

This receiver is a small ESP8266-hosted page at `http://ESP_IP/receive`.
It has exactly two main UI elements:

1. **Received text** — a read-only text area that appends each line entered in the ESP8266 Serial Monitor.
2. **Peer status** — initially hardcoded to **Connected** for the first prototype.

The browser opens one WebSocket connection to `ws://ESP_IP:81/`. New text is pushed as soon as a newline-terminated line is entered in Serial Monitor. There is no periodic polling and no page refresh.

## Files

- `Reciver_Module.ino` — main ESP8266 firmware (sketch filename uses the spelling requested)
- `receiver_ui.h` — HTML embedded in ESP flash and served at `/receive`
- `index.html` — editable UI source
- `styles.css` — editable styling source
- `app.js` — WebSocket client source

## Arduino setup

1. Install the ESP8266 board package in Arduino IDE.
2. Install **WebSockets** by Markus Sattler (Links2004) using Library Manager.
3. Open `Reciver_Module.ino` from this folder. Keep `receiver_ui.h` beside it.
4. Set `WIFI_SSID` and `WIFI_PASSWORD` near the top of the sketch.
5. Select your NodeMCU ESP8266 board and correct port, then upload.
6. Open Serial Monitor at **115200 baud** and set **Line ending: Newline**.
7. Open the IP printed in Serial Monitor followed by `/receive`.
8. Type text into Serial Monitor's input box and press Send. It should appear in the webpage immediately.

## WebSocket event format

The ESP sends one initial status snapshot to each newly connected browser:

```json
{"type":"status","connected":true}
```

When a message is typed in Serial Monitor, it broadcasts:

```json
{"type":"message","text":"Hello from Serial Monitor"}
```

Status changes are broadcast only when the value changes. Use `setPeerConnected(true)` or `setPeerConnected(false)` from your future ESP-to-ESP connection detection code. Calling it repeatedly with the existing state sends nothing.

**Important:** `peerConnected` is deliberately hardcoded to `true` in this first prototype, as requested. It does not yet detect the physical connection to the other ESP8266. The status displayed in the page is the sender/peer ESP link status, not the browser WebSocket connection status.

## Notes

- Serial Monitor input is line-based. Use **Newline** as the line ending. The maximum message length is 500 bytes.
- The UI files are provided separately for editing, but the ESP serves the self-contained HTML/CSS/JavaScript embedded in `receiver_ui.h`. After editing `index.html`, `styles.css`, or `app.js`, regenerate/update `receiver_ui.h` before uploading.
- The page uses WebSocket reconnect events if the browser connection drops. This is event-driven reconnect behavior, not periodic message polling.
