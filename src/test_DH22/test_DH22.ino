#include "DHT.h"
#include <MKRWAN.h>

// Broche DHT
#define DHTPIN 16 // D16
// Definit le type de capteur utilise
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  String TemperatureValue = String(dht.readTemperature());
  String HumidityValue = String(dht.readHumidity());

  Serial.println("Temperature = " + TemperatureValue +" °C");
  Serial.println("Humidite = " + HumidityValue +" %");
  // Attend 1 secondes avant de reboucler
  delay(1000);

}
