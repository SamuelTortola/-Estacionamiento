#include <WiFi.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "ESP32-Final_p";
const char* password = "123456789";

// Página HTML con WebSocket para actualizar la interfaz en tiempo real
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Terminal de Parqueo</title>
  <style>
    body { 
      font-family: Arial, sans-serif; 
      display: flex; 
      flex-direction: column; 
      align-items: center; 
      justify-content: center; 
      height: 100vh; 
      margin: 0; 
      background-color: #f4f4f4; 
    }

    .titulo {
      font-size: 36px;
      font-weight: bold;
      text-align: center;
      color: #4CAF50;
      margin-top: 20px;
      text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3);
      padding: 10px;
      width: 100%;
      background-color: #333;
      color: white;
    }

    .main-container {
      display: flex;
      justify-content: flex-start;
      align-items: center;
      gap: 40px;
      margin-top: 30px;
    }
    
    .content { 
      display: flex; 
      flex-direction: column; 
      align-items: center; 
      margin-right: 20px; 
    }
    
    h1 { color: #333; margin: 20px 0; font-size: 2em; }
    
    .carros-container { 
      display: grid; 
      grid-template-columns: repeat(4, 120px); 
      grid-template-rows: repeat(2, 180px); 
      gap: 15px; 
    }

    .carro { 
      width: 120px; 
      height: 180px; 
      border-radius: 10px; 
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); 
      display: flex; 
      justify-content: center; 
      align-items: center; 
      font-size: 36px; 
      font-weight: bold; 
      color: white; 
      transition: background-color 0.3s; 
    }

    .carro-verde { background-color: #4CAF50; }
    .carro-rojo { background-color: red; }

    /* Estilo para el cuadrado que muestra el número de carros verdes */
    .cuadrado {
      width: 200px;
      height: 250px;
      display: flex;
      justify-content: center;
      align-items: center;
      font-size: 200px;
      font-weight: bold;
      color: red;
      background-color: black;
      border-radius: 10px;
    }
  </style>
</head>
<body>
  <div class="titulo">
    TERMINAL DE PARQUEO
  </div>
  
  <div class="main-container">
    <div class="content">
      <div class="carros-container">
        <div id="carro1" class="carro carro-verde">1</div>
        <div id="carro2" class="carro carro-verde">2</div>
        <div id="carro3" class="carro carro-verde">3</div>
        <div id="carro4" class="carro carro-verde">4</div>
        <div id="carro5" class="carro carro-verde">5</div>
        <div id="carro6" class="carro carro-verde">6</div>
        <div id="carro7" class="carro carro-verde">7</div>
        <div id="carro8" class="carro carro-verde">8</div>
      </div>
    </div>

    <!-- Cuadrado que muestra el número de carros verdes -->
    <div class="cuadrado" id="cuadrado">
      0
    </div>
    
  </div>

  <script>
    const carros = [...document.querySelectorAll(".carro")];
    const cuadrado = document.getElementById("cuadrado");

    const ws = new WebSocket(`ws://${window.location.hostname}/ws`);
    ws.onmessage = (event) => {
      const { status, stateDis } = JSON.parse(event.data);
      
      // Actualización de carros
      status.forEach((state, i) => {
        carros[i].classList.toggle("carro-rojo", state === 1);
        carros[i].classList.toggle("carro-verde", state === 0);
      });

      // Actualización del cuadrado
      updateSquare(stateDis);
    };

    function updateSquare(stateDis) {
      // Mostrar el número de carros verdes en el cuadrado
      cuadrado.textContent = stateDis;
    }
  </script>
</body>
</html>


)rawliteral";

bool carStatus[8] = {false, false, false, false, false, false, false, false}; // Estados de los carros
int State_Dis = 0;  // Estado inicial del display (0 carros en verde)
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");  // WebSocket para actualizaciones en tiempo real

void setup() {
  Wire.begin(0x55);                    // Configura el ESP32 como esclavo en I2C con dirección 0x01
  Wire.onReceive(receiveEvent);        // Llama a receiveEvent al recibir datos I2C
  Serial.begin(115200);
  
  // Configuración del AP Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.print("IP del AP: ");
  Serial.println(WiFi.softAPIP());

  // Configuración del servidor WebSocket
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Configuración del servidor HTTP
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

// Función para recibir eventos I2C
void receiveEvent(int bytes) {
  while (Wire.available()) {
    int carNumber = Wire.read() - 1;  // Convertir valor a índice (0 a 7 para carros 1 a 8)
    if (carNumber >= 0 && carNumber < 8) {
      carStatus[carNumber] = !carStatus[carNumber]; // Alternar el estado del carro
      notifyClients(); // Notificar a los clientes sobre el cambio
    }
  }
}

// Notificar a los clientes WebSocket del cambio de estado
void notifyClients() {
  String message = "[";
  for (int i = 0; i < 8; i++) {
    message += carStatus[i] ? "1" : "0";
    if (i < 7) message += ",";
  }
  message += "]";

  // Contamos los carros en verde
  int greenCarsCount = 0;
  for (int i = 0; i < 8; i++) {
    if (carStatus[i] == false) { // Si el carro está en verde (estado 0)
      greenCarsCount++;
    }
  }
  Serial.print(greenCarsCount);// Ver en terminal


  // Enviar el estado y la cantidad de carros verdes
  String stateMessage = "{\"status\":" + message + ", \"stateDis\":" + String(greenCarsCount) + "}";
  ws.textAll(stateMessage);
}

// Función para manejar eventos del WebSocket
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == AwsEventType::WS_EVT_CONNECT) {
    Serial.println("Nuevo cliente conectado");
  } else if (type == AwsEventType::WS_EVT_DISCONNECT) {
    Serial.println("Cliente desconectado");
  }
}

void loop() {
  // No es necesario código adicional en loop, ya que el servidor WebSocket maneja los cambios
}
