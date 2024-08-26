// Biblioteca para permitir la comunicación serie en pines diferentes a los pines 0 y 1
#include <SoftwareSerial.h>

// Pines para el motor paso a paso
const int pasoPin = 2;  // Pin para la dirección
const int dirPin = 3;   // Pin para la dirección
const int enPin = 12;   // Pin para habilitar el driver

// Pines para los sensores PIR
int sensorPIR = 4;
int sensorPIR2 = 7;

// Variables para almacenar el estado de movimiento detectado por los sensores PIR
int movimiento;
int movimiento2;

// Pin para el sensor Hall
int sensorHall = 11;

// Pasos necesarios para una revolución completa de la llave
const int pasoXRevolcion = 1400;

// Variable de control para el estado de la puerta
bool tope = false;

// Pines para el LED RGB
const int pinVerde = 5;
const int pinAzul = 6;
const int pinRojo = 9;

// Configuración del puerto serie para el ESP8266
SoftwareSerial espSerial(10, 8);  // TX, RX

void setup() {
  Serial.begin(115200);     // Inicializa la comunicación serie con el monitor serie
  espSerial.begin(115200);  // Inicializa la comunicación serie con el módulo ESP8266

  Serial.println(
    "SISTEMA DE ACCESO AUTOMÁTICO CONTROLADO MEDIANTE "
    "PLATAFORMA WEB PARA UN SALÓN DOMOTIZADO\n");

  // Inicialización de los pines del motor paso a paso
  pinMode(dirPin, OUTPUT);
  pinMode(pasoPin, OUTPUT);

  // Inicialmente deshabilitamos el driver
  digitalWrite(enPin, HIGH);

  // Inicialización de los pines de los sensores PIR
  pinMode(sensorPIR, INPUT);
  pinMode(sensorPIR2, INPUT);

  // Inicialización de los pines del LED RGB
  pinMode(pinRojo, OUTPUT);
  pinMode(pinVerde, OUTPUT);
  pinMode(pinAzul, OUTPUT);

  // Apaga el LED RGB al inicio
  encenderColor(0, 0, 0);

  // Inicialización del pin del sensor Hall
  pinMode(sensorHall, INPUT);

  // Envia comandos AT para inicializar el módulo ESP8266
  enviarComandoAT("AT\r\n", 2000);                                // Verifica la comunicación con el módulo
  enviarComandoAT("AT+CWMODE=2\r\n", 1000);                       // Configura el modo Wi-Fi (2: Modo AP)
  enviarComandoAT("AT+CWSAP=\"G4\",\"12345678\",5,3\r\n", 2000);  // Configura SSID, contraseña y tipo de codificación (3: 4 conexiones maximo)
  enviarComandoAT("AT+CIPMUX=1\r\n", 1000);                       // Habilita multiples conexiones (1: WPA2_PSK)
  enviarComandoAT("AT+CIPSERVER=1,80\r\n", 1000);                 // Configura como servidor en el puerto 80
  enviarComandoAT("AT+CIFSR\r\n", 1000);                          // Muestra su dirección IP
}

void loop() {

  // Verifica si hay datos disponibles del ESP8266
  if (espSerial.available() > 0) {

    // Lee la línea de datos
    String linea = espSerial.readStringUntil('\n');

    // Busca la posición del primer "/"
    int indiceBarra = linea.indexOf('/');
    if (indiceBarra != -1) {  // Si se encontró "/"

      // Extrae la parte de la cadena después de la primera "/"
      String dato = linea.substring(indiceBarra + 1);

      // Busca el siguiente espacio para obtener solo el dato después del "/"
      int indiceEspacio = dato.indexOf(' ');
      if (indiceEspacio != -1) {
        dato = dato.substring(0, indiceEspacio);
      }

      // Comando para cerrar la puerta
      if (dato == "Cerrar" && tope == false) {
        Serial.println("Cerrar");
        Serial.println("Verificando que nadie esté en el aula...");

        // Enciende el LED RGB de color amarillo para indicar que esta en estado de verificacación del aula
        encenderColor(100, 30, 0);

        // Verifica si hay movimiento en el aula durante 10 segundos
        for (int i = 0; i < 10; i++) {

          movimiento = digitalRead(sensorPIR);
          movimiento2 = digitalRead(sensorPIR2);

          if (movimiento == 1 || movimiento2 == 1) {
            Serial.println("Hay alguien en el G4\n");

            alertarPresencia();  // Parpadea el LED RGB de color rojo
            break;               // Rompe el ciclo
          }
          delay(1000);
        }

        // Verifica si no hay movimiento para cerrar la puerta
        if (movimiento == 0 && movimiento2 == 0) {

          Serial.println("Verificando si la puerta está lista para cerrar...");

          // Verifica si la puerta está en la posición adecuada
          if (digitalRead(sensorHall) == LOW) {

            Serial.println("\nPuerta lista para cerrar");

            // Habilita el driver para energizar el motor
            digitalWrite(enPin, LOW);
            delay(100);

            // Define el giro del eje del motor hacia la izquierda para cerrar
            digitalWrite(dirPin, HIGH);
            // delay(1300);

            Serial.println("Cerrando...");

            // Ciclo de pasos para el cierre de la cerradura
            for (int x = 0; x < pasoXRevolcion; x++) {
              digitalWrite(pasoPin, HIGH);
              delay(2);
              digitalWrite(pasoPin, LOW);
              delay(2);
            }

            // Deshabilita el driver para desenergizar el motor
            digitalWrite(enPin, HIGH);

            tope = true;
            Serial.println("Cerrado\n");

            encenderColor(0, 0, 255);  // Enciende el LED RGB de color azul
          } else {
            Serial.println("\nLa puerta no está en posición para cerrar");
            Serial.println("Por favor, verifica la posición de la puerta, por favor\n");
            alertarPosicionIncorrecta();
          }
        }
      }

      // Comando para abrir la puerta
      else if (dato == "Abrir" && tope == true) {
        Serial.println("Abrir");

        // Habilita el driver para energizar el motor
        digitalWrite(enPin, LOW);
        delay(100);

        // Define el giro del eje del motor hacia la derecha para abrir
        digitalWrite(dirPin, LOW);

        Serial.println("Abriendo...");

        // Ciclo de pasos para la apertura de la cerradura
        for (int x = 0; x < pasoXRevolcion; x++) {
          digitalWrite(pasoPin, HIGH);
          delay(2);
          digitalWrite(pasoPin, LOW);
          delay(2);
        }

        // Deshabilita el driver para desenergizar el motor
        digitalWrite(enPin, HIGH);

        tope = false;
        Serial.println("Abierto\n");

        encenderColor(0, 255, 0);  // Enciende el LED RGB de color verde
      }
    }
  }
}

// Función para encender el LED RGB con ciertos valores de rojo, verde y azul
void encenderColor(int rojo, int verde, int azul) {
  analogWrite(pinRojo, 255 - rojo);
  analogWrite(pinVerde, 255 - verde);
  analogWrite(pinAzul, 255 - azul);
}

// Parpadea el LED RGB de color rojo para alertar presencia en el aula
void alertarPresencia() {
  for (int i = 0; i < 5; i++) {
    encenderColor(255, 0, 0);  // Enciende el LED RGB de color rojo por medio segundo
    delay(500);
    encenderColor(0, 0, 0);  // Se apaga el LED RGB por medio segundo
    delay(500);
  }
  encenderColor(255, 0, 0);  // El LED RGB se queda encendido de color rojo por 5 segundos
  delay(5000);
  encenderColor(0, 255, 0);  // El LED RGB vuelve a encenderse de color verde
}

// Parpadea el LED RGB de color naranja para alertar presencia en el aula
void alertarPosicionIncorrecta() {
  for (int i = 0; i < 5; i++) {
    encenderColor(100, 10, 0);  // Enciende el LED RGB de color rojo por medio segundo
    delay(500);
    encenderColor(0, 0, 0);  // Se apaga el LED RGB por medio segundo
    delay(500);
  }
  encenderColor(100, 10, 0);  // El LED RGB se queda encendido de color naranaja por 5 segundos
  delay(5000);
  encenderColor(0, 255, 0);  // El LED RGB vuelve a encenderse de color verde
}

// Parpadea el LED RGB de color azul para indicar el cierre de la puerta
void indicarCierre() {
  encenderColor(0, 0, 255);  // Enciende el LED RGB de color azul
  delay(1000);               // Espera de un segundo
  encenderColor(0, 0, 0);    // Se apaga el LED RGB
}

// Parpadea el LED RGB de color verde para indicar la apertura de la puerta
void indicarApertura() {
  encenderColor(0, 255, 0);  // Enciende el LED RGB de color verde
  delay(1000);               // Espera de un segundo
  encenderColor(0, 0, 0);    // Se apaga el LED RGB
}

// Envia comandos al ESP8266 y verifica la respuesta del módulo, todo esto dentro del tiempo timeout
void enviarComandoAT(String comando, const int timeout) {
  long int time = millis();  // Mide el tiempo actual para verificar timeout

  espSerial.print(comando);  // Envia el comando al ESP8266

  while ((time + timeout) > millis()) {  // Mientras no haya timeout
    while (espSerial.available()) {      // Mientras haya datos por leer

      // Leer los datos disponibles
      char c = espSerial.read();  // Lee el siguiente caracter
      Serial.print(c);
    }
  }
  return;
}
