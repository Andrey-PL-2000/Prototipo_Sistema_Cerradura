// Pines para el LED RGB
const int pinVerde = 5;
const int pinAzul = 6;
const int pinRojo = 9;

void setup() {

  // Configura los pines como salida
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);
}

void loop() {

  // Rojo
  encenderColor(10, 0, 0); // Enciende el LED en rojo
  delay(1000); // Espera 1 segundo
  
  // Verde
  encenderColor(0, 10, 0); // Enciende el LED en verde
  delay(1000); // Espera 1 segundo
  
  // Azul
  encenderColor(0, 0, 10); // Enciende el LED en azul
  delay(1000); // Espera 1 segundo
  
  // Naranja
  encenderColor(10, 1, 0); // Enciende el LED en naranja
  delay(1000); // Espera 1 segundo
  
  // Amarillo
  encenderColor(10, 3, 0); // Enciende el LED en amarillo
  delay(1000); // Espera 1 segundo
}

// Función para encender el LED RGB con ciertos valores de rojo, verde y azul
void encenderColor(int rojo, int verde, int azul) {
  analogWrite(pinRojo, 255 - rojo);
  analogWrite(pinVerde, 255 - verde);
  analogWrite(pinAzul, 255 - azul);
}
