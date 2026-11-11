# ESP8266 Plain-Text Sender

This version intentionally does **not** encrypt messages. The browser sends the textarea contents directly as a `text/plain` HTTP POST to the ESP8266.

## What it does
1. Join the same Wi-Fi network configured in `Sender_Module.ino`.
2. Open `http://ESP8266_IP/msg` in a browser.
3. Type up to 500 characters and click **Send Message**.
4. The browser sends `POST /msg` with the raw text in the request body.
5. The ESP8266 prints that text in Arduino Serial Monitor.

## Upload
- Keep `Sender_Module.ino` and `sender_ui.h` in the same Arduino sketch folder. The folder name should match the `.ino` filename (`Sender_Module`).
- Select your ESP8266 board and correct port in Arduino IDE.
- Upload the sketch.
- Open Serial Monitor at **115200 baud**.
- Copy the IP address printed after Wi-Fi connects, then visit `http://<IP>/msg`.

## Request details
- Page: `GET /msg`
- Message: `POST /msg`
- Header: `Content-Type: text/plain;charset=UTF-8`
- Body: the message text itself (not JSON and not encrypted)
- Firmware reads the body with `server.arg("plain")` and prints it with `Serial.println(message)`.

## Important
Because this is plain text, anyone able to observe the network traffic may be able to read the message. Use this only on a network you trust. The included image is embedded in `sender_ui.h`, so the ESP8266 does not need to serve a separate image file. `index.html`, `styles.css`, `app.js`, and `assets/` are also included for editing/previewing the frontend separately.
