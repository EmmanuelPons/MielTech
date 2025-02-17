#include "HX711.h"
HX711 scale; // DT, CLK                        

float calibration_factor = -3.7;
float units;
float ounces;

void setup() {
   Serial.begin(9600);
   scale.begin(2, 3);  // Définition des broches DT et CLK
   Serial.println("HX711 calibration croquis");
   Serial.println("Remove all weight from scale");
   Serial.println("Press + or a to increase calibration factor");
   Serial.println("Press - or z to decrease calibration factor");

   scale.set_scale();
   scale.tare();

   long zero_factor = scale.read_average();
   Serial.print("Zero factor: ");
   Serial.println(zero_factor);
}

void loop() {
   scale.set_scale(calibration_factor);

   Serial.print("Reading: ");
   units = scale.get_units(), 10;
   if (units < 0) { units = 0.00; }
   ounces = units * 0.035274;

   Serial.print(ounces);
   Serial.print(" grams"); 
   Serial.print(" calibration_factor: ");
   Serial.print(calibration_factor);
   Serial.println();

   if (Serial.available())
   {
       char temp = Serial.read();
       if (temp == '+' || temp == 'a')
          calibration_factor += 1;
       else if (temp == '-' || temp == 'z')
          calibration_factor -= 1;
   }
}