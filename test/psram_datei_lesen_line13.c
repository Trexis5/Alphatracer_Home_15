void datei_lesen_line13() {
    uint16_t zeilenZaehler = 0;
    int i = 0;
    File file = SPIFFS.open(dateiGr13, "r");

    if (!file) {
        #ifdef NVS_MQTT_DEBUG
            Serial.println("[FI] Die Datei " + String(dateiGr13) + " existiert nicht!");
        #endif
        File file = SPIFFS.open(dateiGr13, FILE_WRITE);
        #ifdef NVS_MQTT_DEBUG
            Serial.println("[FI] Datei " + String(dateiGr13) + " wurde erstellt!");
        #endif
        file.print("0");
        file.close();
        file = SPIFFS.open(dateiGr13, "r");
    }

    #ifdef NVS_MQTT_DEBUG
        Serial.println("[FI] Lese Daten aus MQTT Datei:");
    #endif

    char* data = (char*)ps_malloc(256);  // 256 Bytes von PSRAM zuweisen

    if (data == NULL) {
        Serial.println("PSRAM allocation failed!");
        return;
    }

    while (file.available()) {
        int len = file.readBytesUntil('\n', data, 256);
        data[len] = '\0';  // Null-terminieren

        #ifdef NVS_MQTT_DEBUG
            Serial.print("[FI] r Zeile ");
            Serial.print(zeilenZaehler);
            Serial.print(": ");
            Serial.println(data);  // ... und wieder ausgegeben
        #endif

        i++;
        zeilenZaehler++;
    }

    charcount13 = zeilenZaehler;
    charcount13_t = charcount13;

    #ifdef NVS_MQTT_DEBUG
    Serial.println("[FI] Aktueller MQTT read Zeilen Zähler: " + String(charcount13));
    #endif

    file.close();
    free(data);  // PSRAM freigeben
}

