// #include "Arduino.h"

// // GRUPO EMISSOR - PAR

// #define PINO_RX 13
// #define PINO_TX 13
// #define RTS 12  // emissor->receptor
// #define CTS 11  // receptor->emissor
// #define BAUD_RATE 1
// #define HALF_BAUD 1000 / (2 * BAUD_RATE)

// #include <Temporizador.h>

// int currentBit = -1;  // bit sendo tratado durante a execução
// bool flagTemporizador = false;
// char currentChar = '\0';  // variável auxiliar para ler o caracter que está
//                           // sendo transmitido durante a execução

// bool bitParidade(char);

// // Rotina de interrupcao do timer1
// // O que fazer toda vez que 1s passou?
// ISR(TIMER1_COMPA_vect) {
//     // Primeira itereção
//     if (currentBit == -1) digitalWrite(PINO_TX, HIGH);

//     // verificando os dados
//     else if (currentBit >= 0 && currentBit < 8)
//         digitalWrite(PINO_TX, bitRead(currentChar, currentBit) ? HIGH : LOW);

//     // bit de paridade
//     else if (currentBit == 8)
//         digitalWrite(PINO_TX, bitParidade(currentChar) ? HIGH : LOW);

//     // verifica se houve o encerramento da transmissão
//     else if (currentBit == 9)
//         digitalWrite(PINO_TX, HIGH);

//     else if (currentBit > 9) {
//         digitalWrite(RTS, LOW);
//         flagTemporizador = false;
//         paraTemporizador();
//     }

//     // se a transmissão não acabou, seguir para o próximo bit
//     if (currentBit <= 9) {
//         currentBit++;
//         Serial.print(digitalRead(PINO_TX));
//     }
// }

// // Calcula bit de paridade - Par ou impar
// bool bitParidade(char dado) {
//     int count = 0;
//     for (int i = 0; i < 8; i++) {
//         count += bitRead(dado, i);
//     }
//     return !(count % 2);  // par -> resto 0 -> complemento de 0 -> 1
// }

// // Executada uma vez quando o Arduino reseta
// void setup() {
//     // Removendo as interrupções de clock
//     noInterrupts();

//     // Setando a porta serial (Serial Monitor - Ctrl + Shift + M)
//     Serial.begin(9600);

//     // Inicializando os pinos
//     pinMode(PINO_TX, OUTPUT);

//     // pins de handshake:
//     pinMode(RTS, OUTPUT);
//     pinMode(CTS, INPUT);

//     digitalWrite(RTS, LOW);

//     // Setando o temporizador
//     configuraTemporizador(BAUD_RATE);

//     // habilitando novamente interrupcoes
//     interrupts();
// }

// // O loop() eh executado continuamente (como um while(true))
// void loop() {
//     // Verifica se há dados sendo transmitidos
//     if (Serial.available() > 0 && digitalRead(RTS) == LOW &&
//         digitalRead(CTS) == LOW) {
//         currentBit = -1;
//         currentChar = Serial.read();
//         digitalWrite(RTS, HIGH);
//     }

//     if (Serial.available() > 0 && digitalRead(RTS) == HIGH &&
//         digitalRead(CTS) == HIGH) {
//         currentBit = -1;
//         iniciaTemporizador();
//     } else {
//         paraTemporizador();
//     }
// }
