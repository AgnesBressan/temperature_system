#include <DHT.h>

// Configuração do sensor DHT
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Pinos dos LEDs
#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4

float temperatura_simulada = 40.0; // Valor inicial para a temperatura simulada

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Configuração dos LEDs como saída
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  delay(2000); // Dá tempo para o sensor inicializar
}

void loop() {
  // Verificar se há dados recebidos via Serial
  if (Serial.available() > 0) {
    char comando = Serial.read(); // Ler o comando enviado
    if (comando == 'A') {         // Comando para aumentar a temperatura
      temperatura_simulada += 0.5;
    } else if (comando == 'D') {  // Comando para diminuir a temperatura
      temperatura_simulada -= 0.5;
    }
  }

  // Garantir que a temperatura simulada esteja no intervalo [0, 40]
  if (temperatura_simulada > 40.0) {
    temperatura_simulada = 40.0;
  } else if (temperatura_simulada < 0.0) {
    temperatura_simulada = 0.0;
  }

  // Variável para armazenar qual LED será aceso
  String estado_led;

  // Lógica dos LEDs com base na temperatura simulada
  if (temperatura_simulada > 26.6) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    estado_led = "LED Vermelho ligado (Temperatura baixa)";
  } else if (temperatura_simulada > 13.3 && temperatura_simulada <= 26.6) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    estado_led = "LED Amarelo ligado (Temperatura média)";
  } else if (temperatura_simulada <= 13.3) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    estado_led = "LED Verde ligado (Temperatura alta)";
  }

  // Envia as informações ao monitor serial
  Serial.print("Temperatura simulada: ");
  Serial.print(temperatura_simulada);
  Serial.print(" °C\t");
  Serial.println(estado_led);

  delay(100); // Intervalo de leitura
}
