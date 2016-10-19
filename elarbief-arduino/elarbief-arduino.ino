#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>

const long velocidadBaud = 9600;

const int pinMotorL1 = 5;
const int pinMotorL2 = 6;

const int pinMotorR1 = 10;
const int pinMotorR2 = 11;

const int velocidadPwm = 224;

typedef enum { D_ALTO, D_ADELANTE, D_ATRAS, D_IZQUIERDA,
               D_DERECHA
             } DIRECCION;

typedef enum { PA_NINGUNO, PA_DEAMBULAR,
             } PROCESO_AUTOMATICO;

PROCESO_AUTOMATICO procesoActual = PA_NINGUNO;

unsigned int tiempoInicial = 0;
unsigned int tiempoEspera = 0;

#define Tigger     9
#define Echo       8
#define Medicion 200

NewPing sonar(Tigger, Echo, Medicion);

void setup() {
  Wire.begin();
  pinMode(pinMotorL1, OUTPUT);
  pinMode(pinMotorL2, OUTPUT);
  pinMode(pinMotorR1, OUTPUT);
  pinMode(pinMotorR2, OUTPUT);

  Serial.begin(velocidadBaud);
  Serial.println(F("Listo"));
}

void loop() {
  char dato;

  if (Serial.available()) {
    dato = Serial.read();
    procesarComando(dato);
  }

  procesosAutomaticos();
}

void procesarComando(char dato) {
  

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
      procesoActual = PA_NINGUNO;
      break;
    case 'F':
      procesoActual = PA_DEAMBULAR;
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

void procesosAutomaticos() {
  int tiempoActual = millis();

  switch (procesoActual) {
    case PA_NINGUNO:
      break;
    case PA_DEAMBULAR:
      if (tiempoEspera > 0) {
        if (tiempoActual >= tiempoInicial + tiempoEspera)
          tiempoEspera = 0;
      }
      else {
        if (sonar.ping_cm() < 15) {
          controlMotor(D_DERECHA);
          tiempoInicial = tiempoActual;
          tiempoEspera = 500;
        }
        else
          controlMotor(D_ADELANTE);
      }
      break;
  }
}

