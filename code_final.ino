#include <MKRWAN.h>
#include <DHT22.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HX711.h>
#include <ArduinoLowPower.h>

// -----------------------------------------------------------
// 1) Définition des pins & objets
// -----------------------------------------------------------

// DHT22 #1 sur pinDATA1 = 2, DHT22 #2 sur pinDATA2 = 3
#define pinDATA1 2
#define pinDATA2 3
DHT22 dht22_1(pinDATA1);
DHT22 dht22_2(pinDATA2);

// DS18 sur OneWire (pin 21)
#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18(&oneWire);

// Adresses DS18
DeviceAddress sensor1Addr = { 0x28, 0xA4, 0x77, 0x75, 0xD0, 0x01, 0x3C, 0x4B };
DeviceAddress sensor2Addr = { 0x28, 0xBD, 0x96, 0x56, 0xB5, 0x01, 0x3C, 0xF9 };

// HX711 (pins 17 & 18)
HX711 scale;
uint8_t dataPin = 17;
uint8_t clockPin = 18;

// Luminosité (A1)
#define LIGHT_SENSOR_PIN A1
const float VREF    = 3.3;
const int   ADC_RES = 4095;

// Batterie (A0)
const int pin_batt = A0;
const float facteur_division = 0.795;
const float ref_ADC = 3.3;
const int resolution_ADC = 4095;

// LED / régulateur
const int ledPin = LED_BUILTIN;
#define SHDN_PIN 5

// LoRa (MKR WAN)
LoRaModem modem;
String appEui = "0000000000000000";
String appKey = "F9A26DE4BC24DEC9D48AFC60C7198763";

// Variables de connexion
bool connected = false;
int err_count  = 0;
short con      = 0;

void setup() {
  // LED 10s au démarrage
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(10000);
  digitalWrite(ledPin, LOW);

  pinMode(SHDN_PIN, OUTPUT);

  // Initialisation de la liaison série
  Serial.begin(115200);

  // Résolution ADC 12 bits
  analogReadResolution(12);
  Serial.println(" **************************************************** ");
  Serial.println("                Welcome to Miel Tech ");
  Serial.println("                     OPEN RUCHE ");
  Serial.println("     Site : https://sites.google.com/view/mieltech  ");
  Serial.println(" ****************************************************  ");

  // 1) Init LoRa
  if (!modem.begin(EU868)) {
    Serial.println("Erreur init modem LoRa !");
    while (true);
  }
  delay(2000);
  connected = false;
  err_count = 0;
  con = 0;

  // 2) DS18
  ds18.begin();

  // 3) HX711
  scale.begin(dataPin, clockPin);
  scale.set_offset(-8017);
  scale.set_scale(13.550673);

  Serial.println("Capteurs initialisés.");
}

void loop() {
  // -----------------------------------------------------------
  // 1) Activation du régulateur
  // -----------------------------------------------------------
  digitalWrite(SHDN_PIN, HIGH);
  delay(20000); // Stabilisation (capteurs & alim)

  // -----------------------------------------------------------
  // 2) Lecture multiple DHT22 #1
  // -----------------------------------------------------------
  for (int i = 0; i < 3; i++) {
    dht22_1.getTemperature();
    dht22_1.getHumidity();
    delay(2000);
  }
  float fT   = dht22_1.getTemperature();
  float fH   = dht22_1.getHumidity();

  // -----------------------------------------------------------
  // 3) Lecture DHT22 #2 (simple)
  // -----------------------------------------------------------
  float fT_i = dht22_2.getTemperature();
  float fH_i = dht22_2.getHumidity();

  // -----------------------------------------------------------
  // 4) DS18
  // -----------------------------------------------------------
  ds18.requestTemperatures();
  float fT_0 = ds18.getTempC(sensor1Addr);
  float fT_1 = ds18.getTempC(sensor2Addr);

  // -----------------------------------------------------------
  // 5) HX711 => Poids (kg)
  // -----------------------------------------------------------
  float raw = scale.get_units(5);  // Lecture en grammes
  float fWeight_kg = raw / 1000.0; // Ex : 30850 g => 30.85 kg

  // -----------------------------------------------------------
  // 6) Luminosité
  // -----------------------------------------------------------
  int sensorValue = analogRead(LIGHT_SENSOR_PIN); // 0..4095
  unsigned int luxValue = (unsigned int)sensorValue;

  // -----------------------------------------------------------
  // 7) Batterie
  // -----------------------------------------------------------
  int valeur_ADC    = analogRead(pin_batt);
  float tension_ADC = (valeur_ADC * ref_ADC) / resolution_ADC;
  float fBatt       = tension_ADC / facteur_division;

  // -----------------------------------------------------------
  // Vérification des lectures
  // -----------------------------------------------------------
  bool valid = true;
  if (isnan(fT) || isnan(fH) || isnan(fT_i) || isnan(fH_i) ||
      isnan(fT_0) || isnan(fT_1)) {
    valid = false;
  }

  // -----------------------------------------------------------
  // 8) Conversion améliorée
  // -----------------------------------------------------------
  //
  // TEMPERATURES => 1 décimale => ×10
  // ex: 27.5 => 275 (int16)
  int16_t t   = (int16_t)round(fT   * 10);
  int16_t h   = (int16_t)round(fH   * 10);
  int16_t t_i = (int16_t)round(fT_i * 10);
  int16_t h_i = (int16_t)round(fH_i * 10);
  int16_t t_0 = (int16_t)round(fT_0 * 10);
  int16_t t_1 = (int16_t)round(fT_1 * 10);

  // POIDS => 2 décimales => ×100
  // ex: 30.85 => 3085
  int16_t weight = (int16_t)round(fWeight_kg * 100);

  // LUMINOSITE => uint16_t
  uint16_t lux = (uint16_t)luxValue;

  // -----------------------------------------------------------
  // 9) Affichage sur terminal
  // -----------------------------------------------------------
  if (valid) {
    Serial.println("----- Données Capteurs -----");

    // Affichage DHT22 #1 => 1 décimale
    Serial.print("DHT22 #1 => t = ");
    Serial.print(fT, 1); // ex: 27.5
    Serial.print(" °C, h = ");
    Serial.print(fH, 1);
    Serial.println(" %");

    // Affichage DHT22 #2 => 1 décimale
    Serial.print("DHT22 #2 => t = ");
    Serial.print(fT_i, 1);
    Serial.print(" °C, h = ");
    Serial.print(fH_i, 1);
    Serial.println(" %");

    // DS18 => 1 décimale
    Serial.print("DS18 #1 => t = ");
    Serial.println(fT_0, 1);
    Serial.print("DS18 #2 => t = ");
    Serial.println(fT_1, 1);

    // Poids => 2 décimales
    Serial.print("Poids    => ");
    Serial.print(fWeight_kg, 2);
    Serial.println(" kg");

    // Luminosité
    Serial.print("Luminosité => ");
    Serial.println(lux);

    // Batterie => float
    Serial.print("Batterie  => ");
    Serial.print(fBatt, 2); 
    Serial.println(" V");
    Serial.println("----------------------------");
  } else {
    Serial.println("Erreur lecture DHT/DS18 => skip");
  }

  // -----------------------------------------------------------
  // 10) Envoi LoRa
  // -----------------------------------------------------------
  if (valid) {
    // 1) Join si non connecté
    if (!connected) {
      Serial.print("Join test : ");
      Serial.println(++con);
      int ret = modem.joinOTAA(appEui, appKey);
      if (ret == 1) {
        connected = true;
        modem.minPollInterval(60);
        Serial.println("Connected to LoRa!");
        modem.dataRate(5);
        delay(2000);
        err_count = 0;
      }
    }

    if (connected) {
      modem.beginPacket();

      modem.write((byte*)&t,      2);  // temp #1 (×10)
      modem.write((byte*)&h,      2);  // hum #1  (×10)
      modem.write((byte*)&t_i,    2);  // temp #2 (×10)
      modem.write((byte*)&h_i,    2);  // hum #2  (×10)
      modem.write((byte*)&t_0,    2);  // DS18 #1 (×10)
      modem.write((byte*)&t_1,    2);  // DS18 #2 (×10)
      modem.write((byte*)&weight, 2);  // Poids x100
      modem.write((byte*)&lux,    2);  // Luminosité (uint16_t)

      // Batterie en float (4 octets)
      modem.write((byte*)&fBatt, sizeof(float));

      int err = modem.endPacket();
      if (err <= 0) {
        Serial.print("Erreur d'envoi LoRa : ");
        Serial.println(err);
        err_count++;
        if (err_count > 50) {
          connected = false;
        }
        // En cas d'erreur, on attend 120 s
        for (int i = 0; i < 120; i++) {
          delay(1000);
        }
      } else {
        err_count = 0;
        Serial.println("Message LoRa envoyé !");
      }
    }
  }

  // -----------------------------------------------------------
  // Deep Sleep
  // -----------------------------------------------------------
  digitalWrite(SHDN_PIN, LOW);
  LowPower.deepSleep(600000); // 10 min
}
