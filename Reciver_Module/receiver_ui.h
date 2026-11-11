// Auto-generated embedded receiver webpage.
// Source files: index.html, styles.css, app.js
// The ESP8266 serves this page at /receive.
#pragma once
#include <pgmspace.h>

const char receiver_html[] PROGMEM = R"RECEIVERHTML(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="theme-color" content="#f7f5f2">
  <title>ESP Messenger | Receiver</title>
  <style>
@import url('https://fonts.googleapis.com/css2?family=DM+Sans:wght@400;500;600;700;800&display=swap');
:root {
  --paper: #f7f5f2;
  --ink: #1b1d21;
  --muted: #747780;
  --line: #dedbd7;
  --accent: #a34e43;
  --green: #16864a;
  --green-bg: #e9f6ed;
}
* { box-sizing: border-box; }
body {
  margin: 0; min-height: 100vh; background: var(--paper); color: var(--ink);
  font-family: 'DM Sans', Arial, sans-serif;
}
.topbar {
  height: 78px; margin: 0 4%; border-bottom: 1px solid var(--line);
  display: flex; align-items: center; justify-content: space-between;
}
.brand { display: flex; align-items: center; gap: 11px; color: var(--ink); text-decoration: none; font-size: 18px; font-weight: 800; }
.brand-mark { display: grid; place-items: center; width: 36px; height: 36px; border-radius: 9px; color: #fff; background: var(--ink); font-size: 20px; }
.module-label { color: var(--muted); font-size: 11px; font-weight: 700; letter-spacing: 1.7px; }
.page-shell { width: min(900px, 90%); margin: 0 auto; padding: 66px 0 55px; }
.intro { margin-bottom: 34px; }
.eyebrow, .card-kicker { color: #89858a; font-size: 10px; font-weight: 800; letter-spacing: 2px; margin: 0 0 15px; }
h1 { margin: 0; font-size: clamp(39px, 7vw, 64px); line-height: 1.04; letter-spacing: -2.8px; font-weight: 800; }
h1 em { color: var(--accent); font-style: normal; }
.intro-copy { max-width: 570px; margin: 18px 0 0; color: #5f626b; font-size: 16px; line-height: 1.65; }
.receiver-card { background: #fffdfb; border: 1px solid #dcd9d5; border-radius: 19px; padding: 27px; box-shadow: 0 13px 35px #26221e0c; }
.card-top { display: flex; gap: 15px; align-items: center; justify-content: space-between; margin-bottom: 26px; }
.card-kicker { margin-bottom: 6px; font-size: 9px; }
h2 { margin: 0; font-size: 23px; letter-spacing: -.6px; }
.status-pill { display: inline-flex; flex: none; align-items: center; gap: 9px; padding: 9px 13px; background: var(--green-bg); border: 1px solid #b9e1c5; border-radius: 30px; color: var(--green); font-size: 12px; font-weight: 700; }
.status-dot { width: 9px; height: 9px; border-radius: 50%; background: #19a45a; box-shadow: 0 0 0 3px #19a45a1c; }
.status-pill.disconnected { background: #fff0ed; border-color: #edc1b9; color: #a33e32; }
.status-pill.disconnected .status-dot { background: #c34b3d; box-shadow: 0 0 0 3px #c34b3d1c; }
label { display: block; margin: 0 0 10px; font-size: 13px; font-weight: 700; }
textarea { display: block; width: 100%; min-height: 250px; resize: vertical; padding: 17px; border: 1px solid #d8d5d1; border-radius: 12px; background: #faf9f7; color: #292b30; font: 14px/1.7 'DM Sans', Arial, sans-serif; outline: none; }
textarea::placeholder { color: #a1a0a0; }
.card-foot { display: flex; justify-content: space-between; align-items: center; gap: 12px; margin-top: 15px; color: #888b92; font-size: 11px; }
.live-mark { display: inline-flex; align-items: center; gap: 7px; color: #666a72; font-weight: 800; letter-spacing: 1px; font-size: 9px; }
.live-mark i { width: 7px; height: 7px; border-radius: 50%; background: var(--green); }
.page-note { display: flex; gap: 10px; align-items: flex-start; max-width: 700px; margin: 22px auto 0; color: #777a82; font-size: 12px; line-height: 1.6; }
.note-icon { color: var(--accent); font-size: 19px; line-height: 1; }
footer { border-top: 1px solid var(--line); margin: 0 4%; padding: 19px 0 24px; text-align: center; color: #858892; font-size: 11px; }
.footer-separator { padding: 0 9px; }
@media (max-width: 600px) {
  .topbar { height: 68px; }
  .brand { font-size: 15px; }
  .brand-mark { width: 32px; height: 32px; }
  .module-label { font-size: 9px; letter-spacing: 1px; }
  .page-shell { padding: 43px 0 38px; }
  h1 { letter-spacing: -1.8px; }
  .intro-copy { font-size: 14px; }
  .receiver-card { padding: 19px 16px; }
  .card-top { align-items: flex-start; }
  h2 { font-size: 20px; }
  .status-pill { padding: 8px 10px; font-size: 11px; }
  textarea { min-height: 220px; }
}

  </style>
</head>
<body>
  <header class="topbar">
    <a class="brand" href="/receive" aria-label="ESP Messenger receiver home">
      <span class="brand-mark">E</span>
      <span>ESP Messenger</span>
    </a>
    <span class="module-label">RECEIVER MODULE</span>
  </header>

  <main class="page-shell">
    <section class="intro">
      <p class="eyebrow">DEVICE-TO-DEVICE MESSAGING</p>
      <h1>Your messages,<br><em>live as they arrive.</em></h1>
      <p class="intro-copy">Incoming text from your ESP device appears here instantly. No page refresh and no polling.</p>
    </section>

    <section class="receiver-card" aria-label="Receiver interface">
      <div class="card-top">
        <div>
          <p class="card-kicker">INCOMING MESSAGE</p>
          <h2>Message console</h2>
        </div>
        <div class="status-pill" id="peerStatus" aria-live="polite">
          <span class="status-dot"></span>
          <span id="peerStatusText">Connected</span>
        </div>
      </div>

      <label for="receivedText">Received text</label>
      <textarea id="receivedText" readonly spellcheck="false" placeholder="Waiting for a message from the ESP..."></textarea>
      <div class="card-foot">
        <span class="live-mark"><i></i> LIVE RECEIVER</span>
        <span id="messageCount">0 messages received</span>
      </div>
    </section>

    <p class="page-note"><span class="note-icon">⌁</span> This page stays connected to the ESP through WebSocket. New serial input is pushed to this screen automatically.</p>
  </main>

  <footer><span>ESP8266</span><span class="footer-separator">•</span><span>Receiver interface</span></footer>
  <script>
(() => {
  const output = document.getElementById('receivedText');
  const statusPill = document.getElementById('peerStatus');
  const statusText = document.getElementById('peerStatusText');
  const countText = document.getElementById('messageCount');

  let socket = null;
  let reconnectDelay = 1000;
  let messageCount = 0;

  function setPeerStatus(isConnected) {
    // Called only when the ESP sends a status event (or its initial snapshot).
    statusText.textContent = isConnected ? 'Connected' : 'Disconnected';
    statusPill.classList.toggle('disconnected', !isConnected);
  }

  function displayMessage(text) {
    // Append the latest serial message; no timer or periodic page refresh is used.
    output.value += (output.value ? '\n\n' : '') + text;
    output.scrollTop = output.scrollHeight;
    messageCount += 1;
    countText.textContent = `${messageCount} message${messageCount === 1 ? '' : 's'} received`;
  }

  function connectWebSocket() {
    const scheme = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const url = `${scheme}//${window.location.hostname}:81/`;
    socket = new WebSocket(url);

    socket.addEventListener('open', () => {
      reconnectDelay = 1000;
      console.info('[Receiver] WebSocket connected:', url);
    });

    socket.addEventListener('message', (event) => {
      try {
        const packet = JSON.parse(event.data);
        if (packet.type === 'status' && typeof packet.connected === 'boolean') {
          setPeerStatus(packet.connected);
        } else if (packet.type === 'message' && typeof packet.text === 'string') {
          displayMessage(packet.text);
        }
      } catch (error) {
        console.warn('[Receiver] Ignored invalid WebSocket event:', error);
      }
    });

    socket.addEventListener('close', () => {
      console.warn('[Receiver] WebSocket closed; reconnecting.');
      scheduleReconnect();
    });

    socket.addEventListener('error', () => {
      // The close event performs reconnection; no polling is used.
      socket.close();
    });
  }

  function scheduleReconnect() {
    const delay = reconnectDelay;
    reconnectDelay = Math.min(reconnectDelay * 2, 15000);
    window.setTimeout(connectWebSocket, delay);
  }

  connectWebSocket();
})();

  </script>
</body>
</html>

)RECEIVERHTML";
