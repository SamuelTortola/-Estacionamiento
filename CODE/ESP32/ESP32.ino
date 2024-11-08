#include <Wire.h>

void setup() {
    Wire.begin(0x01); // Inicia el ESP32 como esclavo con la dirección ESP32_ADDRESS
    Wire.onReceive(receiveEvent); // Evento al recibir datos
    Wire.onRequest(requestEvent); // Evento al solicitar datos
    Serial.begin(115200); // Iniciar la comunicación serial a 115200 baudios
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
    Wire.write("data from ESP32"); // Envía datos al maestro
}


void loop() {
    // Puedes dejarla vacía si no necesitas hacer nada en el bucle principal
}
