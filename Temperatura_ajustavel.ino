#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inicializa o pino do sensor de temperatura
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Ligar e desligar - Refrigeração e Dejelo
#define PIN_REFRIGERACAO 5
#define PIN_DEJELO 6

// Definição de Max e Min temperatura - Por segurança
int tempMin = -5;
int tempMax = 15;

// Definição de temperatura desejada Inicial
int tempDesejada = 5;

// Definição dos botões de regular a temperatura desejada
#define BOTAO_AUMENTA 8
#define BOTAO_DIMINUI 9

// Variáveis para controle do Dejelo
unsigned long intervaloDejelo = 43200000; // 12 horas em milissegundos
unsigned long lastDejeloTime = 0;
bool dejeloActive = false;

void setup() {
  // inicializa o LCD
  lcd.init();
  lcd.backlight();
  
  // inicializa os pinos dos relés de refrigeração e Dejelo
  pinMode(PIN_REFRIGERACAO, OUTPUT);
  pinMode(PIN_DEJELO, OUTPUT);

  // inicializa os pinos dos botões
  pinMode(BOTAO_AUMENTA, INPUT_PULLUP);
  pinMode(BOTAO_DIMINUI, INPUT_PULLUP);

  // inicializa o sensor de temperatura
  sensors.begin();
}

void loop() {
  // atualiza a temperatura sem maiores adições
  sensors.requestTemperatures(); 
  float temperature = sensors.getTempCByIndex(0);
  
  //Nome escolhido
  lcd.setCursor(0, 0);
  lcd.print("STRAPPER");

  // escreve a temperatura com base no sensor
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  
  // Regula o valor da temperatura desejada - alterar: delay(**) - para aumentar ou diminuir o tempo de resposta
  if (digitalRead(BOTAO_AUMENTA) == LOW) {
    tempDesejada++;
    delay(200);
  }
  
  if (digitalRead(BOTAO_DIMINUI) == LOW) {
    tempDesejada--;
    delay(200);
  }
  
  // escreve a temperatura desejada
  lcd.setCursor(9, 0);
  lcd.print("TD: ");
  lcd.print(tempDesejada);
  lcd.print("C");

  // Verifica se o tempo de espera para o dejelo ja concluiu

if (millis() - lastDejeloTime > intervaloDejelo) {
  digitalWrite(PIN_DEJELO, HIGH);
  delay(1200000);   //Tempo que fica ligado o dejelo (20 min e milissegundos)
  digitalWrite(PIN_DEJELO, LOW);
  lastDejeloTime = millis();
}
 
  // liga ou desliga o relé de refrigeração de acordo com a temperatura desejada
  if (temperature < tempDesejada) {
    digitalWrite(PIN_REFRIGERACAO, HIGH);
  } else {
    digitalWrite(PIN_REFRIGERACAO, LOW);
  }

  // Intervalo de atualização do LCD (atualiza os valores de temperatura com base no sensor)
  delay(2000);
}
