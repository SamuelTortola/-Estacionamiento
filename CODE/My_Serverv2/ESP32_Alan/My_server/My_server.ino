/**
   ******************************************************************************
 Universidad Del Valle De Guatemala
 IE2023: Electrónica digital 2
 Autor: Samuel Tortola - 22094, Alan Gomez - 22115
 Proyecto: Proyecto servidor WEB
 Hardware: STM32
 Creado: 8/11/2024
Última modificación: 9/11/2024
******************************************************************************
  */

// Load Wi-Fi library
#include <WiFi.h>
#include "Wire.h"
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid     = "ESP32-Final_p";
const char* password = "123456789";

volatile uint8_t  Number_Display = 8;
volatile uint8_t  Recived_Car = 1;
bool C1 = false, C2 = false, C3 = false, C4 = false, C5 = false, C6 = false, C7 = false, C8 = false;// Banderas de Carros
bool Bandera = true;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

void setup() {
  Wire.begin(0x01); // Inicia el ESP32 como esclavo con la dirección ESP32_ADDRESS
  Wire.onReceive(receiveEvent); // Evento al recibir datos
  Wire.onRequest(requestEvent); // Evento al solicitar datos
  Serial.begin(115200);
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void receiveEvent(int bytes) {
    while (Wire.available()) {
         int c = Wire.read(); // Lee cada byte recibido como un entero
        Serial.print(c);     // Imprime el valor como número
        Serial.print(" ");   // Espacio entre los números
    }
     Serial.println();
}

void requestEvent() {
    Wire.printf("data from ESP32"); // Envía datos al maestro
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"es\">");
            client.println("<head>");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<title>Terminal con Display de 7 Segmentos</title>");
            client.println("<style>");
            client.println("body {"
                            "font-family: Arial, sans-serif;"
                            "margin: 0;"
                            "padding: 0;"
                            "height: 100vh;"
                            "display: flex;"
                            "flex-direction: column;"
                            "align-items: center;"
                            "justify-content: flex-start;"
                            "background-color: #f4f4f4;"
                          "}");
            client.println(".titulo {"
                            "font-size: 36px;"
                            "font-weight: bold;"
                            "text-align: center;"
                            "color: #4CAF50;"
                            "margin-top: 20px;"
                            "text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3);"
                            "padding: 10px;"
                            "width: 100%;"
                            "background-color: #333;"
                            "color: white;"
                            "position: relative;"
                          "}");
            client.println(".main-container {"
                            "display: flex;"
                            "justify-content: center;"
                            "align-items: center;"
                            "gap: 40px;"
                            "margin-top: 30px;"
                            "flex-grow: 1;"
                          "}");
            client.println(".carros-container {"
                            "display: grid;"
                            "grid-template-columns: repeat(4, 120px);"
                            "grid-template-rows: repeat(2, 180px);"
                            "gap: 15px;"
                          "}");
            client.println(".carro {"
                            "width: 120px;"
                            "height: 180px;"
                            "background-color: #4CAF50;"
                            "border-radius: 10px;"
                            "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"
                            "display: flex;"
                            "justify-content: center;"
                            "align-items: center;"
                            "font-size: 36px;"
                            "font-weight: bold;"
                            "color: white;"
                          "}");
            client.println(".carro-rojo { background-color: red; }");
            client.println(".display {"
                            "width: 150px;"
                            "height: 250px;"
                            "display: grid;"
                            "grid-template-rows: repeat(5, 1fr);"
                            "grid-template-columns: repeat(3, 1fr);"
                            "gap: 5px;"
                            "background-color: black;"
                            "border-radius: 10px;"
                            "padding: 10px;"
                          "}");
            client.println(".segment {"
                            "width: 40px;"
                            "height: 40px;"
                            "background-color: gray;"
                            "border-radius: 5px;"
                            "margin: 2px;"
                          "}");
            client.println(".segment-on { background-color: red; }");
            client.println(".button-container {"
                            "position: absolute;"
                            "bottom: 20px;"
                            "right: 20px;"
                            "display: flex;"
                            "justify-content: center;"
                            "align-items: center;"
                          "}");
            client.println("button {"
                            "padding: 10px 20px;"
                            "font-size: 16px;"
                            "background-color: #4CAF50;"
                            "color: white;"
                            "border: none;"
                            "border-radius: 5px;"
                            "cursor: pointer;"
                            "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);"
                          "}");
            client.println("button:hover { background-color: #45a049; }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.println("<div class=\"titulo\">TERMINAL DE PARQUEO</div>");
            client.println("<div class=\"main-container\">");
            client.println("<div class=\"carros-container\">");
            
            client.println("<div class=\"carro\">1</div>");
            client.println("<div class=\"carro\">2</div>");
            client.println("<div class=\"carro\">3</div>");
            client.println("<div class=\"carro\">4</div>");
            client.println("<div class=\"carro\">5</div>");
            client.println("<div class=\"carro\">6</div>");
            client.println("<div class=\"carro\">7</div>");
            client.println("<div class=\"carro\">8</div>");


            client.println("</div>");
            client.println("<div class=\"display\">");

            client.println("<div class=\"segment segment-off\"></div>");
            client.println("<div class=\"segment segment-off\"></div>");
            client.println("<div class=\"segment segment-on\"></div>");

            client.println("<div class=\"segment segment-off\"></div>");
            client.println("<div class=\"segment segment-on\"></div>");
            client.println("<div class=\"segment segment-on\"></div>");

            client.println("<div class=\"segment segment-on\"></div>");
            client.println("<div class=\"segment segment-off\"></div>");
            client.println("<div class=\"segment segment-on\"></div>");

            client.println("<div class=\"segment segment-off\"></div>");
            client.println("<div class=\"segment segment-off\"></div>");
            client.println("<div class=\"segment segment-on\"></div>");

            client.println("<div class=\"segment segment-on\"></div>");
            client.println("<div class=\"segment segment-on\"></div>");
            client.println("<div class=\"segment segment-on\"></div>");
            
            client.println("</div>");
            client.println("</div>");
            client.println("<div class=\"button-container\">");
            client.println("<button>Botón</button>");
            client.println("</div>");
            client.println("</body>");
            client.println("</html>");
            
            // The HTTP response ends with another blank line
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
  delay(1000);
}
