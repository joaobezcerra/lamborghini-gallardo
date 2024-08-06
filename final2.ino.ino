#include <Ultrasonic.h>
#include <Servo.h>

#define pinoServo 7
#define Trig 2
#define Echo 3
#define B1A 8
#define B1B 9
#define A1A 10
#define A1B 11

int distanciaD;
int distanciaE;
int buzzerPin = 6;

float distanciaObstaculo = 35;

Ultrasonic ultrasonico(Trig, Echo);
Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(pinoServo);
  
  pinMode(B1A, OUTPUT);
  pinMode(B1B, OUTPUT);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(buzzerPin, OUTPUT); 
  
  servo.write(90);
}

void loop() {
  int frente = 1;
  int tras = 2;
  int direita = 3;
  int esquerda = 4;
  int parar = 5;

  while (true) {
    // Anda para frente até detectar um obstáculo
    while (ultrasonico.Ranging(CM) > distanciaObstaculo) {
      Andar(frente);
    }
    
    // Se detectar um obstáculo, para e dá ré em curta distância
    Andar(parar);
    delay(500);
    Andar(tras);
    delay(300);
    Andar(parar);
    delay(1000);
    
    // Gira o sensor ultrassônico para a direita e esquerda
    int statuss = Radar();
    delay(500);
    
    // Realiza ações com base na decisão do sensor ultrassônico
    if (statuss == direita) {
      Andar(direita);
      delay(600);
      Andar(frente);
    } else if (statuss == esquerda) {
      Andar(esquerda);
      delay(600);
       Andar(frente);
    } else {
      Andar(frente);
    }
    delay(1000);
  }
}

void Andar(int direcao) {
  if (direcao == 1) { // anda pra frente
    digitalWrite(B1A, HIGH);
    digitalWrite(B1B, LOW);
  } else if (direcao == 2) { // anda pra trás
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, HIGH);
  } else if (direcao == 3) { // faz curva pra direita
    digitalWrite(B1A, HIGH);
    digitalWrite(B1B, LOW);
    digitalWrite(A1A, HIGH);
    digitalWrite(A1B, LOW);
  } else if (direcao == 4) { // faz curva pra esquerda
    digitalWrite(B1A, HIGH);
    digitalWrite(B1B, LOW);
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, HIGH);
  } else if (direcao == 5) { // FREIA
    digitalWrite(B1A, LOW);
    digitalWrite(B1B, LOW);
  }
}

int Radar() {
  delay(1000);
  servo.write(175);
  delay(1000);
  distanciaD = ultrasonico.Ranging(CM);
  
  delay(1000);
  servo.write(10);
  delay(1000);
  distanciaE = ultrasonico.Ranging(CM);
  
  delay(1000);
  servo.write(90);
  
  if (distanciaD > distanciaE) {
    return 3; // se tiver espaço na direita
  } else if (distanciaD < distanciaE) {
    return 4; // se tiver espaço na esquerda
  } else {
    return 0;
  }
}
