#include "Arduino.h"

// Receptor - Paridade Par - Grupo

#define PINO_RX 13
#define PINO_TX 13
#define RTS 12  // emissor->receptor
#define CTS 11  // receptor->emissor
#define BAUD_RATE 1
#define HALF_BAUD 1000 / (2 * BAUD_RATE)

#include <Temporizador.h>

int current_bit = 0;  // bit sendo tratado durante a execução
bool flagTemporizador = false;
char currentChar = '\0';  // variável auxiliar para ler o carácter que está
                          // sendo transmitido durante a execução

int buffer[11];
bool stop_bit, parity, start_bit, check_pb;

bool bitParidade(char);
void transmissionError();
char interpretBuffer(int[]);

// Rotina de interrupção do timer1
// O que fazer toda vez que 1s passou?

// 0    1    2    3    4    5    6    7    8    9    10   (len = 11)
// SB  LSB   D2   D3   D4   D5   D6   D7   MSB  PB   EB
ISR(TIMER1_COMPA_vect) {
    // Primeira iteração
    if (current_bit == 0)
        while (digitalRead(PINO_RX) == HIGH) start_bit = digitalRead(PINO_RX);

    // verificando os dados
    else if (current_bit >= 1 && current_bit < 9)
        buffer[current_bit] = digitalRead(PINO_RX);

    // bit de paridade
    else if (current_bit == 9) {
        parity = digitalRead(PINO_RX);
        // check_pb = bitParidade(buffer);
        // if (!check_pb) {
        //     transmissionError() /
        // }
    }

    // verifica se houve o encerramento da transmissão
    else if (current_bit == 10) {
        stop_bit = digitalRead(PINO_RX);
        paraTemporizador();
    }
    //
    if (current_bit <= 10) {
        current_bit++;
    } else {
        current_bit = 0;
    }
}
int binaryToDecimal(char binaryNumber[]) {
    int bin, dec = 0;

    bin = atoi(binaryNumber);

    for (int i = 0; bin; i++, bin /= 10)
        if (bin % 10) dec += pow(2, i);

    return dec;
}

char interpretBuffer(int buffer[]) {
    char aux[8];
    // Swap data
    for (int i = 0; i < 8; i++) aux[i] = buffer[8 - i] + 48;
    // Get char
    char result = (char)binaryToDecimal(aux);
    // Verify data integrity
    if ((int)bitParidade(result) == buffer[9]) {
        Serial.println("Brabo");
        return result;
    } else {
        Serial.println("Not brabo");
        transmissionError();
        return (char)21;  // [NEGATIVE ACKNOWLEDGE]
    }
}

// int main(void) {

//   int buffer[11] = {0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1};

//   char s = interpretBuffer(buffer);

//   printf("%c", s);

//   return 0;
// }

// Calcula bit de paridade - Par ou impar
bool bitParidade(char dado) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        count += bitRead(dado, i);
    }
    return !(count % 2);  // par -> resto 0 -> complemento de 0 -> 1
}

// Para transmissão
void transmissionError() {
    Serial.println("Erro de transmissão: Bit de paridade");
    digitalWrite(CTS, LOW);
    paraTemporizador();
    current_bit = 0;
}

// Executada uma vez quando o Arduino reseta
void setup() {
    // Removendo as interrupções de clock
    noInterrupts();

    // Setando a porta serial (Serial Monitor - Ctrl + Shift + M)
    Serial.begin(9600);

    // Inicializando os pinos
    pinMode(PINO_RX, INPUT);

    // pins de handshake:
    pinMode(RTS, INPUT);
    pinMode(CTS, OUTPUT);

    digitalWrite(CTS, LOW);

    // Setando o temporizador
    configuraTemporizador(BAUD_RATE);

    // habilitando novamente interrupções
    interrupts();
}

// O loop() eh executado continuamente (como um while(true))
void loop() {
    // RTS em HIGH????
    if (digitalRead(RTS) == HIGH && digitalRead(CTS) == LOW) {
        Serial.println(
            "Transmission initialized. RTS is True. Setting CTS to True.");
        // Configura o contador de bit
        current_bit = 0;
        // Envia a confirmação do Request To Send
        if (digitalRead(CTS) == LOW) digitalWrite(CTS, HIGH);
        // Inicia a leitura
        iniciaTemporizador();
    }
    // Fim da transmissão
    else if (digitalRead(RTS) == LOW && digitalRead(CTS) == HIGH) {
        Serial.println("Mensagem Recebida: ");
        digitalWrite(CTS, LOW);
        paraTemporizador();
    }
}
