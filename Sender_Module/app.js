const form = document.getElementById('sendForm');
const textBox = document.getElementById('message');
const counter = document.getElementById('counter');
const statusBox = document.getElementById('status');
const sendButton = document.getElementById('sendButton');

textBox.addEventListener('input', () => {
  counter.textContent = `${textBox.value.length} / 500`;
});

function setStatus(message, type = '') {
  statusBox.className = `status ${type}`.trim();
  statusBox.textContent = message;
}

form.addEventListener('submit', async (event) => {
  event.preventDefault();
  const message = textBox.value;
  if (!message.trim()) {
    setStatus('Please type a message first.', 'error');
    textBox.focus();
    return;
  }

  sendButton.disabled = true;
  sendButton.textContent = 'Sending...';
  setStatus('Sending plain text to your ESP8266...', 'busy');
  console.group('[ESP Sender] Message send');
  console.log('POST /msg');
  console.log('Plain-text message:', message);

  try {
    // Same-origin POST to this ESP8266. No encryption and no JSON wrapper.
    const response = await fetch('/msg', {
      method: 'POST',
      headers: { 'Content-Type': 'text/plain;charset=UTF-8' },
      body: message
    });
    const reply = await response.text();
    console.log('ESP8266 response:', response.status, reply);
    if (!response.ok) throw new Error(`HTTP ${response.status}: ${reply}`);
    console.log('RESULT: message delivered to ESP8266 handler.');
    setStatus('Message sent. Check the ESP8266 Serial Monitor.', '');
    textBox.value = '';
    counter.textContent = '0 / 500';
  } catch (error) {
    console.error('Send failed:', error);
    setStatus(`Send failed: ${error.message}`, 'error');
  } finally {
    console.groupEnd();
    sendButton.disabled = false;
    sendButton.textContent = '➤  Send Message';
  }
});
