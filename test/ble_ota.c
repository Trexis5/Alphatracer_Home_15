#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <SPIFFS.h>
#include <esp_ota_ops.h>
#include <esp_partition.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define FIRMWARE_FILE          "/firmware.bin"

bool isUpdating = false;
File firmwareFile;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        connected_users++;
        Serial.println(F("[BT] Client connected"));
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        if (connected_users > 0) {
            connected_users--;
        }
        if (isUpdating) {
            firmwareFile.close();
            Serial.println("Firmware file closed");
            performOTAUpdate();  // Start OTA update when the client disconnects
        }
        Serial.println(F("[BT] Client disconnected"));
    }
};

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        Serial.println(F("[BT] BT Connect"));
        std::string rxValue = pCharacteristic->getValue();
        receivedTraffic += rxValue.length();

        if (rxValue.length() > 0) {
            g_bl_connect = true;
            bufferReceive = "";
            for (int i = 0; i < rxValue.length(); i++){
                bufferReceive += String(rxValue[i]);
            }

            if (!isUpdating) {
                if (SPIFFS.exists(FIRMWARE_FILE)) {
                    SPIFFS.remove(FIRMWARE_FILE);
                }
                firmwareFile = SPIFFS.open(FIRMWARE_FILE, FILE_WRITE);
                isUpdating = true;
                Serial.println("Started firmware update");
            }

            firmwareFile.write((uint8_t*)rxValue.data(), rxValue.length());
            Serial.printf("Received %d bytes\n", rxValue.length());

            // Beispiele für spezifische Befehle
            if (strstr(bufferReceive.c_str(), buff_pwd) != NULL) {
                Serial.println("[BT] LOGIN ok");
                bufferReceive = "";
                String tempstr = "LOGIN OK\r\n";
                queueNotification(tempstr.c_str());
                while (sendNotification);
                g_login = true;
            }

            if (strstr(bufferReceive.c_str(), "STATUS") != NULL) {
                bufferReceive = "";
                Serial.println("[BT] Aktion Status");
                send_bl_status = true;
                sendValuesIndividually2();
            }

            if (strstr(bufferReceive.c_str(), "CONNECT:") != NULL) {
                g_connect_start = true;
                Serial.println("[BT] WLAN Change");
                int Laenge = bufferReceive.length();
                int Suche = bufferReceive.indexOf("CONNECT:") + 8;
                bufferReceive = bufferReceive.substring(Suche, Laenge);

                char tempchar[80];
                bufferReceive.toCharArray(tempchar, Laenge + 1);

                char *name = strtok(tempchar, ",|");
                int posval = 0;

                while (name != NULL) {
                    if (posval == 0) {
                        Serial.print("WLAN Nr. ");
                        ist_wlan = atoi(name); 
                        Serial.print(ist_wlan);
                        Serial.print(" | PW: ");
                    }
                    if (posval == 1) {
                        ist_pw = String(name);
                        Serial.print(ist_pw);
                        Serial.println("");
                    }
                    name = strtok(NULL, ",|"); 
                    posval++;
                }
                Serial.println("****g_connect_WLAN*****");
                g_connect_WLAN = true;
                Serial.println("Found: " + String(wlan_found));
                bufferReceive = "";
            }
        }
    }
};

void performOTAUpdate() {
    const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
    if (update_partition == NULL) {
        Serial.println("No OTA partition found");
        return;
    }

    firmwareFile = SPIFFS.open(FIRMWARE_FILE);
    if (!firmwareFile) {
        Serial.println("Failed to open firmware file");
        return;
    }

    esp_ota_handle_t ota_handle;
    esp_err_t err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &ota_handle);
    if (err != ESP_OK) {
        Serial.println("esp_ota_begin failed");
        return;
    }

    while (firmwareFile.available()) {
        uint8_t data[1024];
        int len = firmwareFile.read(data, sizeof(data));
        if (len > 0) {
            err = esp_ota_write(ota_handle, data, len);
            if (err != ESP_OK) {
                Serial.println("esp_ota_write failed");
                return;
            }
        }
    }

    firmwareFile.close();
    err = esp_ota_end(ota_handle);
    if (err != ESP_OK) {
        Serial.println("esp_ota_end failed");
        return;
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK) {
        Serial.println("esp_ota_set_boot_partition failed");
        return;
    }

    Serial.println("Firmware update successful, restarting...");
    esp_restart();
}

void setup() {
    Serial.begin(115200);
    SPIFFS.begin(true);

    BLEDevice::init("ESP32_BLE");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pTxCharacteristic = pService->createCharacteristic(
                          CHARACTERISTIC_UUID_TX,
                          BLECharacteristic::PROPERTY_NOTIFY
                        );

    pTxCharacteristic->addDescriptor(new BLE2902());

    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
                          CHARACTERISTIC_UUID_RX,
                          BLECharacteristic::PROPERTY_WRITE
                        );

    pRxCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();
    BLEDevice::startAdvertising();
    Serial.println("BLE server started");
}

void loop() {
    // Keine Notwendigkeit für zusätzliche Logik hier
}

