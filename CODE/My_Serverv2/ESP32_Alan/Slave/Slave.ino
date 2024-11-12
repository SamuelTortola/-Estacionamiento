#include "Wire.h"

#define I2C_DEV_ADDR 0x55

uint32_t i = 0;

void onRequest() {
  Wire.print(i++);
  Wire.print(" Packets.");
  Serial.println("onRequest");
  Serial.println();
}

void onReceive(int len) {
  Serial.printf("onReceive[%d]: ", len);
  while (Wire.available()) {
    uint8_t received = Wire.read();  // Lee el dato recibido
    
    // Si el valor recibido es 1, muestra 'A' en UART
    if (received == 1) {
      Serial.println("A");
    } else {
      Serial.write(received);  // Muestra cualquier otro dato recibido
    }
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Wire.onReceive(onReceive);  // Configura la función para cuando se recibe algo
  Wire.onRequest(onRequest);  // Configura la función para cuando se solicita algo
  Wire.begin((uint8_t)I2C_DEV_ADDR);  // Inicia el esclavo con la dirección 0x55
}

void loop() {
  // No es necesario hacer nada en el loop
}
