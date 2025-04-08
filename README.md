# MielTech – Ruche Connectée LoRa

Ce projet vise à concevoir une ruche connectée basse consommation, basée sur un microcontrôleur MKR WAN 1300 et des capteurs environnementaux. Il permet la surveillance à distance de ruches via LoRaWAN et la plateforme TTN/BEEP.

---

## 🚀 Fonctionnalités principales

- Mesure de température et d’humidité (intérieure et extérieure)
- Surveillance du poids de la ruche via une cellule de charge
- Lecture de la luminosité ambiante
- Suivi de l’état de la batterie
- Envoi de données toutes les 10 minutes via LoRa (OTAA)
- Intégration directe avec The Things Network et la plateforme BEEP
- Optimisation de la consommation via deep sleep et régulateur contrôlé

---

## 🔧 Matériel utilisé

- Arduino MKR WAN 1300
- 2x DHT22
- 2x DS18B20 (capteurs de température OneWire)
- HX711 + cellule de charge
- Capteur de luminosité (LDR)
- Mesure batterie via pont diviseur sur A0
- Antenne LoRa + boîtier étanche + batterie LiPo
- Régulateur REG09B avec commande SHDN

---

## 🧠 Structure du code (Arduino)

- Lecture des capteurs
- Conversion des données : int16_t ou uint16_t, float pour batterie
- Encodage en payload LoRa de 20 octets + 4 octets (float)
- Transmission OTAA via `MKRWAN.h`
- Extinction des capteurs et mise en veille 10 minutes via `ArduinoLowPower`

Voir le fichier : [`mieltech_final.ino`](./mieltech_final.ino)

---

## 📦 Payload TTN (decodeUplink.js)

Le fichier [`payload_mieltech.js`](./payload_mieltech.js) décode les 9 valeurs envoyées :

| Variable    | Format     | Unité réelle   |
|-------------|------------|----------------|
| `t`         | int16 /10  | °C             |
| `h`         | int16 /10  | %HR            |
| `t_i`       | int16 /10  | °C             |
| `h_i`       | int16 /10  | %HR            |
| `t_0`       | int16 /10  | °C             |
| `t_1`       | int16 /10  | °C             |
| `weight_kg` | int16 /100 | kg             |
| `l`         | uint16     | brut           |
| `bv`        | float      | Volts          |

Ce décodeur est à intégrer dans TTN ou The Things Stack V3. Il renvoie un objet JSON compatible avec la plateforme BEEP.

---

## 📡 Réseau & Plateformes

- **Réseau LoRa :** The Things Network (EU868)
- **Activation :** OTAA (App EUI + App Key)
- **Visualisation :** https://beep.nl/platform (via webhook TTN → BEEP)

---

## 📍 Localisation
Projet réalisé à **Polytech Sorbonne – Sorbonne Université**, site de **Saint-Cyr-l'École**.

Encadrant : Yann Douze  
Étudiants : Vedat, Enes, Hakan, Emmanuel

---

## 📘 Références

- [Arduino MKR WAN 1300 Documentation](https://docs.arduino.cc/hardware/mkr-wan-1300)
- [The Things Network Docs](https://www.thethingsnetwork.org/docs/)
- [HX711 SparkFun Guide](https://learn.sparkfun.com/tutorials/load-cell-hookup-guide)
- [BEEP Platform](https://beep.nl/platform)

---

## 🛠️ Reproduction du projet

1. Flasher le code dans la carte MKR WAN 1300
2. Connecter tous les capteurs conformément au schéma de câblage
3. Vérifier la calibration HX711
4. S’assurer de la couverture LoRa (TTN)
5. Ajouter le decodeUplink dans la console TTN
6. Connecter TTN à BEEP via webhook

Pour plus d’infos : https://sites.google.com/view/mieltech

---

© 2025 — Projet MielTech — Licence libre Creative Commons BY-NC-SA
