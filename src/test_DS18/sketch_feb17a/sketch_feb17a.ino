// Inclusion de la bibliothèque
#include <OneWire.h>

// Connecteur numérique relié à DQ
const byte DS_PIN = 7;


// Création de l'objet associé au DS18B20
OneWire ds(DS_PIN);


//
// Fonction de lecture de la température
//
void getTemperature(float *temperature) {
 byte data[9], addr[8]; // Données et Adresse du module DS18B20
 
 // Réinitialisation du bus
 ds.reset_search();

 // Recherche du capteur
 if (!ds.search(addr)) return;
 
 // Vérification de l'adresse reçue
 if (OneWire::crc8(addr, 7) != addr[7]) return;
 
 // Vérification du modèle de capteur (DS18B20)
 if (addr[0] != 0x28) return;

 // Sélection du capteur
 ds.reset();
 ds.select(addr);
 
 // Lancement de la mesure
 ds.write(0x44, 1);
 delay(800);
 
 // Resélection du capteur
 ds.reset();
 ds.select(addr);
 
 // Envoie d'une demande de lecture
 ds.write(0xBE);
 
 // Lecture des données
 for (byte i = 0; i < 9; i++) {
 data[i] = ds.read();
 }
 
 // Conversion des données en degrés Celcius
 *temperature = ((data[1] << 8) | data[0]) * 0.0625;
}



void setup() {
 Serial.begin(115200);
 }



void loop() {
 float temperature;
 
 // Lecture de la température
 getTemperature(&temperature);
 
 // Affichage de la température
 Serial.print(F("Temperature : "));
 Serial.print(temperature, 1);
 Serial.write(176); // Caractère degré
 Serial.write('C');
 Serial.println();
 }