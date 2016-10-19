#include <Wire.h>
#include <AltSoftSerial.h>
#include <Servo.h>
#include <NewPing.h>

AltSoftSerial serieBt;
const long velocidadBaud = 9600;

const int pinMotorL1 = 5;
const int pinMotorL2 = 6;

const int pinMotorR1 = 10;
const int pinMotorR2 = 11;

const int velocidadPwm = 224;

typedef enum { D_ALTO, D_ADELANTE, D_ATRAS, D_IZQUIERDA,
               D_DERECHA
             } DIRECCION;

#define Tigger   7
#define Echo     6
#define Medicion 200

NewPing sonar(Tigger, Echo, Medicion);

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
    case 'A': //adelante
      controlMotor(D_ADELANTE);
      break;
    case 'B': //atras
      controlMotor(D_ATRAS);
      break;
    case 'C': //izquierda
      controlMotor(D_IZQUIERDA);
      break;
    case 'D': //derecha
      controlMotor(D_DERECHA);
      break;
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'E':
      controlMotor(D_ALTO);
      break;

  }
}

void controlMotor(DIRECCION dir) {
  switch (dir) {
    case D_ALTO:
      digitalWrite(pinMotorL1, LOW);
      digitalWrite(pinMotorL2, LOW);
      //analogWrite(pinMotorLP, 0);

      digitalWrite(pinMotorR1, LOW);
      digitalWrite(pinMotorR2, LOW);
      //analogWrite(pinMotorRP, 0);
      break;
    case D_ADELANTE:
      digitalWrite(pinMotorL1, HIGH);
      digitalWrite(pinMotorL2, LOW);
      //analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, HIGH);
      digitalWrite(pinMotorR2, LOW);
      //analogWrite(pinMotorRP, velocidadPwm);
      break;
    case D_ATRAS:
      digitalWrite(pinMotorL1, LOW);
      digitalWrite(pinMotorL2, HIGH);
      //analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, LOW);
      digitalWrite(pinMotorR2, HIGH);
      //analogWrite(pinMotorRP, velocidadPwm);
      break;
    case D_IZQUIERDA:
      digitalWrite(pinMotorL1, LOW);
      digitalWrite(pinMotorL2, HIGH);
      //analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, HIGH);
      digitalWrite(pinMotorR2, LOW);
      //analogWrite(pinMotorRP, velocidadPwm);
      break;
    case D_DERECHA:
      digitalWrite(pinMotorL1, HIGH);
      digitalWrite(pinMotorL2, LOW);
      //analogWrite(pinMotorLP, velocidadPwm);

      digitalWrite(pinMotorR1, LOW);
      digitalWrite(pinMotorR2, HIGH);
      //analogWrite(pinMotorRP, velocidadPwm);
      break;
  }
}
