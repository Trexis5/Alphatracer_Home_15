#include <esp_heap_caps.h>

// Definiere sendValueStr2 im PSRAM
String* sendValueStr2;

void setup() {
    // Initialisiere den PSRAM-Heap
    sendValueStr2 = new (heap_caps_malloc(sizeof(String), MALLOC_CAP_SPIRAM)) String();
}

void lesen_next_cloud() {
    *sendValueStr2 = p_inputcloudpre;

    *sendValueStr2 += p_inputcloudchain;
    *sendValueStr2 += "0=";
    // Date
    *sendValueStr2 += String(epochTime_mqtt);
    // Radon value
    *sendValueStr2 += p_inputcloudchain;
    *sendValueStr2 += "1=";
    *sendValueStr2 += String(mb_radon_live_32bit);
    
    // MAC
    *sendValueStr2 += p_inputcloudchain;
    *sendValueStr2 += "2=";
    *sendValueStr2 += WiFi.macAddress();

    // Tracer ID value
    *sendValueStr2 += p_inputcloudchain;
    *sendValueStr2 += "3=";
    *sendValueStr2 += device_id_s;

    // Radon value
    *sendValueStr2 += p_inputcloudchain;
    *sendValueStr2 += "4=";
    *sendValueStr2 += String(radon_mittelwert_24h);

    // Radon value
    *sendValueStr2 += p_inputcloudchain;
    *sendValueStr2 += "5=";
    *sendValueStr2 += String(radon_mittelwert_long);

    // Radon value
    *sendValueStr2 += p_inputcloudchain;
    *sendValueStr2 += "6=";
    *sendValueStr2 += String(mb_radon_save_eintrarge);

    //cloud_list_t = *sendValueStr2;
}

void loop() {
    // Deine Logik hier
}

