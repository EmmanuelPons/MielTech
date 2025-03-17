uint8_t Data = 20;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int valeurBrute = analogRead(Data);  // Lire la valeur sur A0
    float voltage = valeurBrute * (3.3 / 1023.0);  // Convertir en tension (V)
    
    // Conversion en LUX (exemple de courbe)
    float lux = 500 * pow(voltage, -1.5);  
    
    Serial.print("Tension: ");
    Serial.print(voltage);
    Serial.print(" V, LUX: ");
    Serial.println(lux);
    
    delay(500);
}
