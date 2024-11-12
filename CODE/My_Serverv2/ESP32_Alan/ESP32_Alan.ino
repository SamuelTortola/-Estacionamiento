#include <Wire.h>

#define I2C_SLAVE_ADDR 0x08  // Dirección I2C del esclavo

void setup() {
  Wire.begin(I2C_SLAVE_ADDR);  // Configura el ESP32 como esclavo
  Serial.begin(115200);        // Inicia UART para la comunicación con la computadora
}

void loop() {
  // Verifica si hay datos disponibles en el bus I2C
  if (Wire.available() > 0) {
    uint8_t receivedData = Wire.read();  // Lee el dato recibido
    if (receivedData == 0x01) {
      Serial.println("Recibido: 0x01");  // Envía el dato por UART
    }
  }
  delay(10);  // Pequeña espera para evitar bucles excesivos
}
