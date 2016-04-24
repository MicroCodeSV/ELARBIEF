#include <Wire.h>
#include <AltSoftSerial.h>

AltSoftSerial serieBt;

const int pinMotorL1 = 2;
const int pinMotorL2 = 4;
const int pinMotorLP = 3;

const int pinMotorR1 = 7;
const int pinMotorR2 = 5;
const int pinMotorRP = 6;

const int pinTemp = A0;

const long velocidadBaud = 9600;

typedef enum { D_ALTO, D_ADELANTE, D_ATRAS, D_IZQUIERDA,
               D_DERECHA
             } DIRECCION;

const int velocidadPwm = 224;

const long tiempoAvance = 1000;
const long tiempoRotacion = 750;

void setup() {
  Wire.begin();
  pinMode(pinMotorL1, OUTPUT);
  pinMode(pinMotorL2, OUTPUT);
  pinMode(pinMotorR1, OUTPUT);
  pinMode(pinMotorR2, OUTPUT);

  Serial.begin(velocidadBaud);
  serieBt.begin(velocidadBaud);
  Serial.println(F("Listo"));
}

void loop() {
  char dato;

  if (Serial.available()) {
    dato = Serial.read();
    //serieBt.write(dato);
    procesarComando(dato);
  }
  if (serieBt.available()) {
    dato = serieBt.read();
    Serial.write(dato);
    procesarComando(dato);
  }
}

void procesarComando(char dato) {
  float tension;
  float temp;

  switch (dato) {
    case '1': //izquierda
      controlMotor(D_IZQUIERDA);
      delay(tiempoRotacion);
      controlMotor(D_ALTO);
      break;
    case '2': //derecha
      controlMotor(D_DERECHA);
      delay(tiempoRotacion);
      controlMotor(D_ALTO);
      break;
    case '3': //atras
      controlMotor(D_ATRAS);
      delay(tiempoAvance);
      controlMotor(D_ALTO);
      break;
    case '4': //adelante
      controlMotor(D_ADELANTE);
      delay(tiempoAvance);
      controlMotor(D_ALTO);
      break;
    case '5': //Temperatura
      tension = analogRead(pinTemp) * 4.65 / 1023.0;
      temp = (tension - 0.5) * 100.0;
      //Serial.println(tension);
      Serial.println(temp);
      break;
    case '6':
      leerCompas();
      break;
  }
}

void controlMotor(DIRECCION dir) {
  switch (dir) {
    case D_ALTO:
      digitalWrite(pinMotorL1, LOW);
      digitalWrite(pinMotorL2, LOW);
      analogWrite(pinMotorLP, 0);

      digitalWrite(pinMotorR1, LOW);
      digitalWrite(pinMotorR2, LOW);
      analogWrite(pinMotorRP, 0);
      break;
    case D_ADELANTE:
      digitalWrite(pinMotorL1, HIGH);
      digitalWrite(pinMotorL2, LOW);
      analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, HIGH);
      digitalWrite(pinMotorR2, LOW);
      analogWrite(pinMotorRP, velocidadPwm);
      break;
    case D_ATRAS:
      digitalWrite(pinMotorL1, LOW);
      digitalWrite(pinMotorL2, HIGH);
      analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, LOW);
      digitalWrite(pinMotorR2, HIGH);
      analogWrite(pinMotorRP, velocidadPwm);
      break;
    case D_IZQUIERDA:
      digitalWrite(pinMotorL1, LOW);
      digitalWrite(pinMotorL2, HIGH);
      analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, HIGH);
      digitalWrite(pinMotorR2, LOW);
      analogWrite(pinMotorRP, velocidadPwm);
      break;
    case D_DERECHA:
      digitalWrite(pinMotorL1, HIGH);
      digitalWrite(pinMotorL2, LOW);
      analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, LOW);
      digitalWrite(pinMotorR2, HIGH);
      analogWrite(pinMotorRP, velocidadPwm);
      break;
  }
}

int leerCompas() {
  int lectura = 0;
  Wire.beginTransmission(0x42 >> 1);
  Wire.write('A');
  Wire.endTransmission();

  delay(10);

  Wire.requestFrom(0x42 >> 1, 2);
  if (Wire.available() >= 2) {
    lectura = Wire.read();
    lectura <<= 8;
    lectura |= Wire.read();
    lectura /= 10;
    Serial.println(lectura);
  }
}
