#include <WiFi.h>
#include <WebServer.h>

// (ESP32 creates its own Wi-Fi)
const char* ssid = "ESP32-LED";
const char* password = "12345678";

//80 is the default HTTP port
WebServer server(80);

// connects esp32 pins to the UNO

//rawliteral is used to write multi-line strings without needing to escape quotes
//c++ allows raw string and reduces the need for escaping quotes

void website() {
  String html  = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" type="text/css" href="/style.css">
    <title>ESP32 LED Control</title>
    <style>
        label{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            color: white;
            font-size: 20px;
        }
        
        body{
            height: auto;
            width: auto;
            text-align: center;
            background-color: black;
            align-items: center;
            
        }
        
        .toggle-btn {
        
            width: 250px;
            padding: 15px;
            font-size: medium;
            border: none;
            border-radius: 5px;
            margin:20px auto;

            
            
        }
        .btn-on {
            background-color: #f50101ff;
            color: white;
        }
        .btn-off {
            background-color: #53f227ff;
            color: white;

        }
        .toggle-btn {
            padding: 15px;
            font-size: medium;
            border: none;
            border-radius: 5px;
            margin:20px auto;

            
            
        }
        .door-open {
            background-color: #4CAF50;
            color: white;
        }
        .door-close {
            background-color: #f44336;
            color: white;
        }
    </style>
</head>
<body>
    <h2>ESP32 LED Control</h2>
    <label>Click the button to toggle the lights <br>
    <button id="ledBtn" class="toggle-btn btn-off">LED OFF</button>
    <br><label>Click the button to operate the door <br>
    <button id="servoBtn"class="toggle-btn btn-off">DOOR OFF</button>
  
    <script>
    // gets button id
    const button = document.getElementById('ledBtn');
    const servo = document.getElementById('servoBtn');
    let led = false;
    let door= false;

    button.addEventListener('click', function() {//eventlistener
        led = !led;
        
        if (led) {
            fetch('/on');
            button.textContent = 'LED OFF';
            button.className = 'toggle-btn btn-on';
        } else {
            fetch('/off');
            button.textContent = 'LED ON';
            button.className = 'toggle-btn btn-off';
        }
    }
);
    servo.addEventListener('click', function() {//eventlistener
        door = !door;
        
        if (door) {
            fetch('/open');
            servo.textContent = 'DOOR OPEN';
            servo.className = 'toggle-btn door-open';
        } else {
            fetch('/close');
            servo.textContent = 'DOOR CLOSE';
            servo.className = 'toggle-btn door-close';
        }
    }
    );
    </script>
</body>
</html>

)rawliteral";
server.send(200, "text/html", html);//sends the html page to the browser
}

void handleOn() {
    Serial2.write("A");
    server.send(200, "text/plain", "LED is ON");
}

void handleOff() {
    Serial2.write("B");
    server.send(200, "text/plain", "LED is OFF");
}

void openDoor() {
    Serial2.write("C");
    server.send(200, "text/plain", "Door is OPEN");
}

void closeDoor() {
    Serial2.write("D");
    server.send(200, "text/plain", "Door is CLOSED");
}

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RX2, TX2); // RX, TX
    //Serial2 is used because you can assign the RX and TX pins to any pin 
    //Serial 1 is often used for FLASH so we dont use that 
    // Mode 1: ESP32 creates hotspot
    WiFi.softAP(ssid, password);
    Serial.println("Access Point Started");
    Serial.println(WiFi.softAPIP());

  
    //server.on is used to define what should happen when someone visits it 
    server.on("/", website);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.on("/open", openDoor);
    server.on("/close", closeDoor);
  
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}