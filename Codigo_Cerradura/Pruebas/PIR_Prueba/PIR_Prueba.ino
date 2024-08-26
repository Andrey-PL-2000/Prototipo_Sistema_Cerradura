// Pines para los sensores PIR
int sensorPIR = 4;
int sensorPIR2 = 7;

// Variables para almacenar el estado de movimiento detectado por los sensores PIR
int movimiento;
int movimiento2;

void setup() {

  // Inicialización de los pines de los sensores PIR
  pinMode(sensorPIR, INPUT);
  pinMode(sensorPIR2, INPUT);

}

void loop() {

  movimiento = digitalRead(sensorPIR);
  movimiento2 = digitalRead(sensorPIR2);

  // Si cualquiera de los dos sensores detecta movimiento, se imprime el mensaje en el puerto serial
  if (movimiento == 1 || movimiento2 == 1) {
    Serial.println("Hay movimiento");
    delay(500);
  }
}
