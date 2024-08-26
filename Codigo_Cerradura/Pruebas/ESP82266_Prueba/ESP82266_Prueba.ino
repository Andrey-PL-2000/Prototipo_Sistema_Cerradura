void setup() {
  Serial.begin(115200);     // Inicializa la comunicación serie con el monitor serie
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c);
  }
  Serial.println("Hola Muuuundooo!");
  delay(1500);
}