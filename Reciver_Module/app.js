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
