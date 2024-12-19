void datei_lesen_line14b() {
    uint16_t zeilenZaehler = 0;
    uint16_t t_zeilenZaehler = 0;
    uint16_t maxZeilen = 10010; // Maximale Anzahl der zu übertragenden Werte
    File file = SPIFFS.open(dateiGr14, "r");

    char* tempstr = (char*)ps_malloc(4096); // PSRAM für temporäre Zeichenkette zuweisen (Größe anpassen nach Bedarf)
    if (tempstr == NULL) {
        Serial.println("PSRAM allocation failed!");
        return;
    }
    tempstr[0] = '\0'; // Sicherstellen, dass die Zeichenkette initial leer ist

    if (!file) {
        #ifdef NVS_14_DEBUG
            Serial.println(F("[FI] Die Datei ") + String(dateiGr14) + F(" existiert nicht!"));
        #endif 
        File file = SPIFFS.open(dateiGr14, FILE_WRITE);
        #ifdef NVS_14_DEBUG
            Serial.println(F("[FI] Datei ") + String(dateiGr14) + F(" wurde erstellt!"));
        #endif
        file.print("0");
        file.close();
        file = SPIFFS.open(dateiGr14, "r");      
    }

    if (g1415Side != 0) { 
        #ifdef NVS_14_DEBUG
            //start_counter ++;
            //Serial.print(F("[FI] Lese Daten aus Datei Nr.14b ab Zeile ")); Serial.println(start_counter); //start_counter = g14Filewrite;
            //start_counter --;
            //Serial.print(F("Aufzeichnungen "));Serial.print(g1415_Sum_Counter);Serial.print(F(" | g1415_full "));Serial.print(g1415_full);Serial.print(F(" | g1415Side "));Serial.println(g1415Side);  
        #endif
    } else {
        start_counter = 0;
        #ifdef NVS_14_DEBUG
            //Serial.print(F("[FI] Lese alle Daten aus Datei Nr.14b    Aufzeichnungen vorhanden: ")); Serial.println(g14Filewrite); 
            //Serial.print(F(" | g1415_full "));Serial.print(g1415_full);Serial.print(F(" | g1415Side "));Serial.println(g1415Side);  
        #endif
    }          

    char* data = (char*)ps_malloc(256); // PSRAM für Datenzeile zuweisen
    if (data == NULL) {
        Serial.println("PSRAM allocation failed!");
        free(tempstr); // Sicherstellen, dass bereits zugewiesener Speicher freigegeben wird
        return;
    }

    while (file.available() && zeilenZaehler < maxZeilen) {
        int len = file.readBytesUntil('\n', data, 256);
        data[len] = '\0'; // Null-terminieren

        // Jahr überprüfen und korrigieren
        String correctedData = checkAndCorrectDate(String(data));

        if (correctedData.length() > 0) { // Nur gültige Einträge hinzufügen
            zeilenZaehler++;
            t_zeilenZaehler++;
            strcat(tempstr, correctedData.c_str());

            if (t_zeilenZaehler < 10) {
                strcat(tempstr, ";");
            }

            if (t_zeilenZaehler >= 10) {
                strcat(tempstr, "\r\n");
                pTxCharacteristic->setValue(tempstr);
                pTxCharacteristic->notify();
                size_t message_length = strlen(tempstr);
                sentTraffic += message_length;
                delay(20);
                t_zeilenZaehler = 0;
                tempstr[0] = '\0'; // Zeichenkette zurücksetzen
            }
        }

        esp_task_wdt_reset();
    }

    if (t_zeilenZaehler > 0 && zeilenZaehler <= maxZeilen) {
        strcat(tempstr, "\r\n");
        pTxCharacteristic->setValue(tempstr);
        pTxCharacteristic->notify();
        size_t message_length = strlen(tempstr);
        sentTraffic += message_length;
    }

    file.close();
    free(tempstr); // PSRAM freigeben
    free(data); // PSRAM freigeben

    Serial.print(F("Total corrections made: "));
    Serial.println(correctionCount);
}

