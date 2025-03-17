const int pin_batt = A0;
const float facteur_division = 0.795;  // Ajusté avec les mesures réelles
const float ref_ADC = 3.3;
const int resolution_ADC = 4095;
const int ledPin = LED_BUILTIN;

void setup() {
    pinMode(ledPin, OUTPUT); // Définit la LED comme sortie
    digitalWrite(ledPin, HIGH); // Allume la LED
    delay(3000); // Attente de 3 secondes
    digitalWrite(ledPin, LOW); // Éteint la LED
    Serial.begin(115200);
    analogReadResolution(12);
}

void loop() {
    int valeur_ADC = analogRead(pin_batt);
    float tension_ADC = (valeur_ADC * ref_ADC) / resolution_ADC;
    float tension_batt = tension_ADC / facteur_division;

    Serial.print("Valeur ADC : ");
    Serial.print(valeur_ADC);
    Serial.print(" - Tension mesurée sur A0 : ");
    Serial.print(tension_ADC);
    Serial.print("V - Tension batterie réelle : ");
    Serial.print(tension_batt);
    Serial.println("V");

    delay(1000);
}
