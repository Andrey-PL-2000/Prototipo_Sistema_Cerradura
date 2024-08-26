// Pines para el motor paso a paso
const int pasoPin = 2;  // Pin para la dirección
const int dirPin = 3;   // Pin para la dirección
const int enPin = 12;   // Pin para habilitar el driver

// Pasos necesarios para una revolución completa de la llave
const int pasoXRevolcion = 152;

void setup() {

  // Inicialización de los pines del motor paso a paso
  pinMode(dirPin, OUTPUT);
  pinMode(pasoPin, OUTPUT);
}

void loop() {

  // Habilita el driver para energizar el motor
  digitalWrite(enPin, LOW);
  delay(100);

  // Define el giro del eje del motor hacia la izquierda para cerrar
  digitalWrite(dirPin, HIGH);

  // Ciclo de pasos para el cierre de la cerradura
  for (int x = 0; x < pasoXRevolcion; x++) {
    digitalWrite(pasoPin, HIGH);
    delay(4);
    digitalWrite(pasoPin, LOW);
    delay(4);
  }

  // Deshabilita el driver para desenergizar el motor
  digitalWrite(enPin, HIGH);

  delay(1000);

  // Habilita el driver para energizar el motor
  digitalWrite(enPin, LOW);
  delay(100);

  // Define el giro del eje del motor hacia la derecha para abrir
  digitalWrite(dirPin, LOW);

  // Ciclo de pasos para la apertura de la cerradura
  for (int x = 0; x < pasoXRevolcion; x++) {
    digitalWrite(pasoPin, HIGH);
    delay(4);
    digitalWrite(pasoPin, LOW);
    delay(4);
  }

  // Deshabilita el driver para desenergizar el motor
  digitalWrite(enPin, HIGH);

  delay(1000);
} 
