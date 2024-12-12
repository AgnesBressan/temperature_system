#include <DHT.h>

// Configuração do sensor DHT
#define DHTPIN 33
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Pinos dos LEDs
#define LED_VERDE 15
#define LED_AMARELO 16
#define LED_VERMELHO 21

// Pino do potenciômetro
#define POTENCIOMETRO_PIN 34 // Use um pino ADC do ESP32

float temperatura_simulada = 25.0; // Valor inicial para a temperatura simulada

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Configuração dos LEDs como saída
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // Configuração do pino do potenciômetro como entrada analógica
  pinMode(POTENCIOMETRO_PIN, INPUT);

  delay(2000); // Dá tempo para o sensor inicializar
}

void loop() {
  // Leitura da temperatura ambiente pelo DHT
  float t_ambiente = dht.readTemperature();

  if (isnan(t_ambiente)) {
    Serial.println("Erro ao ler o sensor DHT!");
    return;
  }

  // Leitura do potenciômetro e conversão para o intervalo de 0 a 40
  int leitura_pot = analogRead(POTENCIOMETRO_PIN);
  temperatura_simulada = map(leitura_pot, 0, 4095, 0, 40); // Converte a leitura do ADC (0-4095) para o intervalo 0-40°C

  // Lógica dos LEDs com base na temperatura simulada
  String saida = "Temperatura ambiente lida pelo sensor: " + String(temperatura_simulada) + " °C\t";
  //saida += "Temperatura simulada (potenciômetro): " + String(temperatura_simulada) + " °C\t";

  if (temperatura_simulada <= 13.3) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    saida += "LED Verde ligado";
  } else if (temperatura_simulada > 13.3 && temperatura_simulada <= 26.6) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    saida += "LED Amarelo ligado";
  } else if (temperatura_simulada > 26.6) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    saida += "LED Vermelho ligado";
  }

  // Envia as informações ao monitor serial
  Serial.println(saida);

  delay(100); // Intervalo de leitura
}
