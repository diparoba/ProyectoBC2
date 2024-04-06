#include <Servo.h>   // Librería del servomotor 😎
#include <Stepper.h> // Librería del motor paso a paso 👹
#include <tcs3200.h> // Librería del sensor de color ✅

Servo servo;

// Conexión de sensor de color
#define s0 0 
#define s1 1
#define s2 2
#define s3 3
#define out 4

// Conexión del motor a pasos
#define IN1  5
#define IN2  6
#define IN3  7
#define IN4  8

// Define el número de pasos por revolución y crea un objeto Stepper
const int stepsPerRevolution = 250;
Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4);

// Conexión de sensor de barrera
#define presencia 9

// Instancia de la clase tcs3200
tcs3200 tcs(0, 1, 2, 3, 4); // (S0, S1, S2, S3, output pin)

void setup() {
  Serial.begin(9600);
  servo.attach(10);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
  pinMode(presencia, INPUT);
  myStepper.setSpeed(60); // Establece la velocidad del motor 👹 a menor velocidad mayor fuerza del motor
}

void loop() {
  bool estado = digitalRead(presencia);
  if (estado == LOW) {
    int dato = medirColor();
    moverServo(dato);
    /* 
    Al mover el servomotor el motor a pasos deberá mover el objeto 
    para que deje de ser detectado por el sensor de presencia, los
    estudiantes deberán elegir en este ciclo repetitivo cuantas veces
    harán que el motor a pasos avance para que el objeto deje de ser
    detectado por el sensor de presencia.
    
    Esto dependerá tanto de el tamaño de la rueda que mueva la banda
    como de la velocidad establecida para el motor, 😎 es lo que los
    guambras deberán regular para que todo funcione bien 😶‍🌫️.
    */
    delay(1000);
    for(int i = 0 ; i<10 ; i++){
    myStepper.step(stepsPerRevolution);
    }
    delay(1000);  
  }
  else {
    // Realiza un giro completo del motor paso a paso
    myStepper.step(stepsPerRevolution);
  }
}

void moverServo(int n) {
  switch (n) {
    case 1: servo.write(45); break;
    case 2: servo.write(90); break;
    case 3: servo.write(180); break;
    default: servo.write(0); break;
  }
}

int medirColor() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  int red = tcs.colorRead('r');   // Lee el valor de color para rojo
  int green = tcs.colorRead('g');   // Lee el valor de color para verde
  int blue = tcs.colorRead('b');    // Lee el valor de color para azul
  digitalWrite(s3, HIGH);
  digitalWrite(s2, HIGH);
  Serial.print("R:  ");
  Serial.print(red);
  Serial.print("  ");
  Serial.print("G:  ");
  Serial.print(green);
  Serial.print("  ");
  Serial.print("B:  ");
  Serial.print(blue);
  Serial.print("  ");
  if (red < blue && red < green) {
    Serial.println("Rojo");
    return 1;
  }
  else if (blue < red && blue < green) {
    Serial.println("Azul");
    return 3;
  }
  else if (green < red && green < blue) {
    Serial.println("Verde");
    return 2;
  }
  else {
    Serial.println("-");
    return 0;
  }
}
