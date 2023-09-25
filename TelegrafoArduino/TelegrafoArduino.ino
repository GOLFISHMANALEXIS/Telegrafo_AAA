unsigned long signal_len, t1, t2;   // Variables para el tiempo que se presiona un botón
int inputPin = 2;                    // Pin de entrada del pulsador para Morse
int deleteButtonPin = 7;             // Pin de entrada del botón de borrado
int ledPin = 4;                      // Pin de salida para el LED
int buzzerPin = 3;                   // Pin de salida para el buzzer
String code = "";                    // Variable que almacena un alfabeto

void setup() {
  // Se configura la conexión serial y los pines de entrada y salida
  Serial.begin(9600);                // Frecuencia
  pinMode(inputPin, INPUT_PULLUP);    // Ayuda a recibir la entrada en el pin 2
  pinMode(deleteButtonPin, INPUT_PULLUP); // Botón de borrado
  pinMode(ledPin, OUTPUT);            // Pin de salida para el LED
  pinMode(buzzerPin, OUTPUT);         // Pin de salida para el buzzer
}

void loop() {
  // Verificar si se presionó el botón de borrado
  if (digitalRead(deleteButtonPin) == LOW) {
    // Borra el último caracter ingresado
    if (code.length() > 0) {
      code.remove(code.length() - 1);
      Serial.println("Último caracter borrado: " + code);
    }
    delay(500); // Evitar rebotes
  }

  NextDotDash:
  while (digitalRead(inputPin) == HIGH) {}
  t1 = millis();                            // Tiempo al presionar el botón
  digitalWrite(ledPin, HIGH);               // LED encendido mientras se presiona el botón
  digitalWrite(buzzerPin, HIGH);            // Enciende el buzzer al presionar el botón
  while (digitalRead(inputPin) == LOW) {}
  t2 = millis();                            // Tiempo en que no está presionado el botón
  digitalWrite(ledPin, LOW);                // LED apagado cuando no está presionado el botón
  digitalWrite(buzzerPin, LOW);             // Apaga el buzzer cuando no está presionado el botón
  signal_len = t2 - t1;                     // Tiempo total en que es presionado el botón
  if (signal_len > 50)                      // Toma en cuenta el anti rebote del botón
  {
    code += readio();                       // Función para leer punto o guión
  }
  
  // Cambiamos el código para espacio y cambio de línea según la duración de la pulsación
  if (signal_len > 3000) {
    // Solo un cambio de línea sin imprimir nada
    Serial.println();
  } 
  else if (signal_len > 1000) {
    Serial.print(" ");  // Imprime un espacio si la pulsación está entre 1 y 3 segundos
  }
  
  while ((millis() - t2) < 500) {     
    if (digitalRead(inputPin) == LOW) {
      goto NextDotDash;
    }
  }
  convertor();                          // Función para descifrar el código en el alfabeto
}

// Función para la lectura de puntos o guiones
char readio() {
  if (signal_len < 600 && signal_len > 50) {
    return '.';                        // Si el botón es presionado menos de 0.6 segundos, será un punto
  }
  else if (signal_len > 600) {
    return '-';                        // Si el botón es presionado más de 0.6 segundos, será un guión
  }
}

// Función para convertir puntos y guiones en alfabeto
void convertor() {
  // Agrega una condición para verificar si se está borrando
  if (digitalRead(deleteButtonPin) == LOW) {
    // No hagas nada si el botón de borrado está siendo presionado
    return;
  }

  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-l..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
                            }; // Combinaciones para las letras
  int i = 0;
  
  if (code == ".-.-.-") {
    Serial.print(".");
  } 
  else if (code == "-.-.-.") {
    // No imprime nada para el cambio de línea
  }
  else {
    while (letters[i] != "E") {
      if (letters[i] == code) {
        Serial.print(char('A' + i));
        break;
      }
      i++;
    }
  }
  
  code = "";  // Limpiamos el código para la próxima entrada
}
