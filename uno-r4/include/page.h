#pragma once

// The control page served by the UNO R4.
// Every 200 ms it sends the current command to /cmd?c=<letter> and shows the
// sensor line that comes back. Holding a key or button sets the command;
// letting go sets it back to 'x' (stop).
const char PAGE[] = R"html(<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>RedBot</title>
<style>
  body { font-family: sans-serif; text-align: center; }
  button { width: 80px; height: 80px; margin: 4px; font-size: 28px;
           touch-action: none; user-select: none; }
  pre { font-size: 18px; }
</style>
</head>
<body>
<h2>RedBot Control</h2>
<p>Hold W A S D on the keyboard, or press the buttons.</p>
<div><button id="w">W</button></div>
<div><button id="a">A</button><button id="s">S</button><button id="d">D</button></div>
<pre id="sensors">connecting...</pre>

<script>
let cmd = 'x';  // 'x' = stop

// Keyboard: hold a key to move, let go to stop.
document.onkeydown = e => {
  const k = e.key.toLowerCase();
  if ('wasd'.includes(k)) cmd = k;
};
document.onkeyup = () => cmd = 'x';

// On-screen buttons (also work on a phone).
for (const k of 'wasd') {
  const b = document.getElementById(k);
  b.onpointerdown = () => cmd = k;
  b.onpointerup = b.onpointerleave = () => cmd = 'x';
}

// Send the current command, show the sensor reply, repeat.
async function tick() {
  try {
    const res = await fetch('/cmd?c=' + cmd);
    document.getElementById('sensors').textContent = await res.text();
  } catch (e) {
    document.getElementById('sensors').textContent = 'no connection';
  }
  setTimeout(tick, 200);
}
tick();
</script>
</body>
</html>
)html";
