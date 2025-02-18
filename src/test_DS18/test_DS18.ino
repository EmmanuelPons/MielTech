#include <OneWire.h>
#include <DallasTemperature.h>

// Définir le pin de connexion du capteur
#define ONE_WIRE_BUS 21  

// Initialiser OneWire et le capteur de température Dallas
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
    Serial.begin(9600);
    sensors.begin();
}

void loop() {
    sensors.requestTemperatures();  // Demander la température
    float temperatureC = sensors.getTempCByIndex(0);  // Lire la température

    Serial.print("Température: ");
    Serial.print(temperatureC);
    Serial.println(" °C");

    delay(1000);  // Attendre 1 seconde
}
