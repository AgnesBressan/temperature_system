#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Definindo os pinos dos LEDs
#define LED_VERDE 8
#define LED_AMARELO 9
#define LED_VERMELHO 10

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  // Configurando os pinos dos LEDs como saída
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  
  delay(2000);  // Dá tempo para o sensor inicializar
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Inicializa a string para a saída
  String saida = "Humidity: ";
  saida += String(h);
  saida += " %\t";

  // Acender os LEDs com base no intervalo de umidade
  if (h <= 50.5) {
    // Liga o LED Verde, apaga os outros
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    saida += "LED Verde ligado";
  } else if (h > 50.5 && h <= 60.7) {
    // Liga o LED Amarelo, apaga os outros
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    saida += "LED Amarelo ligado";
  } else if (h > 60.7) {
    // Liga o LED Vermelho, apaga os outros
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    saida += "LED Vermelho ligado";
  }

  // Imprime a umidade e qual LED está ligado
  Serial.println(saida);

  delay(100);  // Intervalo de leitura a cada 100 ms
}
