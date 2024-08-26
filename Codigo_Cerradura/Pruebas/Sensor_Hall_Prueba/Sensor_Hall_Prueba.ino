// Pin para el sensor Hall
int sensorHall = 11;

void setup() {

  // Inicialización del pin del sensor Hall
  pinMode(sensorHall, INPUT);

}

void loop() {
  
  // Cuando se detecta un campo magnético, se imprime el mensaje en el puerto serial
  if (digitalRead(sensorHall) == LOW) { 
    Serial.println("Hay un campo magnético");
    delay(100);
  }
}
