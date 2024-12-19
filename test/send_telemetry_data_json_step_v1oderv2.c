void send_telemetry_data_json_step() { 
    Serial.println(F("[SA] Sending Telemetry Data json..."));

    sendTelemetryJson_ok = false;

    char* payload3 = (char*)ps_malloc(Default_Buffering_Size);
    if (payload3 == NULL) {
        Serial.println("PSRAM allocation failed!");
        return;
    }

    snprintf(payload3, Default_Buffering_Size, "{\"devicenumber\":%d,\"radon\":%d}", mb_Seriennummer, mb_radon_live_32bit);

    char attributes[Default_Buffering_Size];
    strncpy(attributes, payload3, Default_Buffering_Size - 1);
    attributes[Default_Buffering_Size - 1] = '\0'; // Sicherstellen, dass der String null-terminiert ist

    send_ueberw_counter_t_start++;
    sendTelemetryJson_ok = tb.sendTelemetryJson(attributes);
    delay(200);

    if (sendTelemetryJson_ok) {
        send_ueberw_counter_t_success++;
        send_client_attribute();
        delay(200);
        send_client2_attribute();
        delay(200);
    } else {
        send_ueberw_counter_t_error++;
        Serial.println(F("[SA] Sending Telemetry Data json fail"));
    }

    float usedRAM = (ESP.getHeapSize() - ESP.getFreeHeap()) / (1024.0 * 1024.0);
    Serial.print(F("Used RAM: ")); Serial.print(usedRAM, 3); Serial.println(F(" MB"));
    Serial.println(F("--------------------------------------------------------"));

    free(payload3); // PSRAM freigeben
}



void send_telemetry_data_json_step() { 
    Serial.println(F("[SA] Sending Telemetry Data json..."));

    sendTelemetryJson_ok = false;

    char* payload3 = (char*)heap_caps_malloc(Default_Buffering_Size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (payload3 == NULL) {
        Serial.println("PSRAM allocation failed!");
        return;
    }

    snprintf(payload3, Default_Buffering_Size, "{\"devicenumber\":%d,\"radon\":%d}", mb_Seriennummer, mb_radon_live_32bit);

    char attributes[Default_Buffering_Size];
    strncpy(attributes, payload3, Default_Buffering_Size - 1);
    attributes[Default_Buffering_Size - 1] = '\0'; // Sicherstellen, dass der String null-terminiert ist

    send_ueberw_counter_t_start++;
    sendTelemetryJson_ok = tb.sendTelemetryJson(attributes);
    delay(200);

    if (sendTelemetryJson_ok) {
        send_ueberw_counter_t_success++;
        send_client_attribute();
        delay(200);
        send_client2_attribute();
        delay(200);
    } else {
        send_ueberw_counter_t_error++;
        Serial.println(F("[SA] Sending Telemetry Data json fail"));
    }

    float usedRAM = (ESP.getHeapSize() - ESP.getFreeHeap()) / (1024.0 * 1024.0);
    Serial.print(F("Used RAM: ")); Serial.print(usedRAM, 3); Serial.println(F(" MB"));
    Serial.println(F("--------------------------------------------------------"));

    heap_caps_free(payload3); // PSRAM freigeben
}

