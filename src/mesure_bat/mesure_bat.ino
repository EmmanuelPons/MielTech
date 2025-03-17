#define VBAT_PIN 15
#define DIVISEUR_RATIO 0.232

void setup() {
    Serial.begin(115200);
    analogReadResolution(12); 
    while (!Serial);  // Attente de l'ouverture du port série
}

void loop() {
    // Lecture de la tension sur A0 (0-4095 pour 0-3.3V)
    int raw = analogRead(VBAT_PIN);
    float vA0 = (raw * 3.3 / 4095.0) ;  // Convertir la lecture ADC en volts
    float vBat = vA0 * DIVISEUR_RATIO; // Compenser le diviseur de tension

    // Déterminer le niveau de charge de la batterie
    int batteryLevel = 0;

    if (vBat >= 4.2) {
        batteryLevel = 100;
    } else if (vBat >= 4.1) {
        batteryLevel = 90;
    } else if (vBat >= 4.0) {
        batteryLevel = 80;
    } else if (vBat >= 3.9) {
        batteryLevel = 70;
    } else if (vBat >= 3.8) {
        batteryLevel = 60;
    } else if (vBat >= 3.7) {
        batteryLevel = 50;
    } else if (vBat >= 3.6) {
        batteryLevel = 40;
    } else if (vBat >= 3.5) {
        batteryLevel = 30;
    } else if (vBat >= 3.4) {
        batteryLevel = 20;
    } else if (vBat >= 3.3) {
        batteryLevel = 10;
    } else {
        batteryLevel = 0; // Batterie critique
    }

    // raw
    Serial.print("valeur raw : ");
    Serial.print(raw);
    Serial.println(" ");

    // Affichage des résultats
    Serial.print("Tension mesurée sur A0 : ");
    Serial.print(vA0);
    Serial.println(" V");

    Serial.print("Tension réelle de la batterie : ");
    Serial.print(vBat);
    Serial.println(" V");

    Serial.print("Niveau de charge de la batterie : ");
    Serial.print(batteryLevel);
    Serial.println(" %");

    // Messages en fonction du niveau de batterie
    if (batteryLevel == 100) {
        Serial.println("🔋 Batterie pleine !");
    } else if (batteryLevel >= 70) {
        Serial.println("🔋 Batterie en bon état.");
    } else if (batteryLevel >= 40) {
        Serial.println("⚠️ Batterie moyenne, pensez à la recharger bientôt.");
    } else if (batteryLevel >= 10) {
        Serial.println("⚠️ Batterie faible !");
    } else {
        Serial.println("❌ Batterie critique, recharge immédiate !");
    }

    Serial.println("--------------------------------------");
    
    delay(5000);  // Rafraîchissement toutes les 5 secondes
}