/*
  Provision_Callback.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Provision_Callback_h
#define Provision_Callback_h

// Local includes.
#include "Callback.h"

// Library includes.
#include <ArduinoJson.h>

// Convenient aliases
using Provision_Data = const JsonObjectConst;

// Struct dispatch tags, to differentiate between constructors, allows the same paramter types to be passed
struct Access_Token{};
struct Device_Access_Token{};
struct Basic_MQTT_Credentials{};
struct X509_Certificate{};

/// @brief Provisioning callback wrapper
class Provision_Callback : public Callback<void, const Provision_Data&> {
  public:
    /// @brief Constructs empty callback, will result in never being called
    Provision_Callback();

    /// @brief Constructs callback that will be fired upon a provision request arrival,
    /// where the requested credentials were sent by the cloud and received by the client.
    /// Using the credentials generated by the ThingsBoard server method. See https://thingsboard.io/docs/user-guide/device-provisioning/?mqttprovisioning=without#mqtt-device-apis
    /// @param cb Callback method that will be called
    /// @param provisionDeviceKey Device profile provisioning key of the device profile that should be used to create the device under
    /// @param provisionDeviceSecret Device profile provisioning secret of the device profile that should be used to create the device under
    /// @param deviceName Name the created device should have on the cloud,
    /// pass nullptr or an empty string if a random string should be used as a name instead
    Provision_Callback(Access_Token, function cb, const char *provisionDeviceKey, const char *provisionDeviceSecret, const char *deviceName = nullptr);

    /// @brief Constructs callback that will be fired upon a provision request arrival,
    /// where the requested credentials were sent by the cloud and received by the client.
    /// Using the device supplies access token method. See https://thingsboard.io/docs/user-guide/device-provisioning/?mqttprovisioning=without#mqtt-device-apis
    /// @param cb Callback method that will be called
    /// @param provisionDeviceKey Device profile provisioning key of the device profile that should be used to create the device under
    /// @param provisionDeviceSecret Device profile provisioning secret of the device profile that should be used to create the device under
    /// @param accessToken Access token generated by the device, that will be used by the provisioned device, alternative to letting the access token be generated by the cloud instead
    /// @param deviceName Name the created device should have on the cloud,
    /// pass nullptr or an empty string if a random string should be used as a name instead
    Provision_Callback(Device_Access_Token, function cb, const char *provisionDeviceKey, const char *provisionDeviceSecret, const char *accessToken, const char *deviceName = nullptr);

    /// @brief Constructs callback that will be fired upon a provision request arrival,
    /// where the requested credentials were sent by the cloud and received by the client.
    /// Using the device supplies basic MQTT credentials method. See https://thingsboard.io/docs/user-guide/device-provisioning/?mqttprovisioning=without#mqtt-device-apis
    /// @param cb Callback method that will be called
    /// @param provisionDeviceKey Device profile provisioning key of the device profile that should be used to create the device under
    /// @param provisionDeviceSecret Device profile provisioning secret of the device profile that should be used to create the device under
    /// @param username Basic MQTT credentials username, that will be used by the provisioned device
    /// @param password Basic MQTT credentials password, that will be used by the provisioned device
    /// @param clientID Basic MQTT credentials clientID, that will be used by the provisioned device
    /// @param deviceName Name the created device should have on the cloud,
    /// pass nullptr or an empty string if a random string should be used as a name instead
    Provision_Callback(Basic_MQTT_Credentials, function cb, const char *provisionDeviceKey, const char *provisionDeviceSecret, const char *username, const char *password, const char *clientID, const char *deviceName = nullptr);

    /// @brief Constructs callback that will be fired upon a provision request arrival,
    /// where the requested credentials were sent by the cloud and received by the client.
    /// Using the device supplies X.509 certificate method. See https://thingsboard.io/docs/user-guide/device-provisioning/?mqttprovisioning=without#mqtt-device-apis
    /// @param cb Callback method that will be called
    /// @param provisionDeviceKey Device profile provisioning key of the device profile that should be used to create the device under
    /// @param provisionDeviceSecret Device profile provisioning secret of the device profile that should be used to create the device under
    /// @param hash Public X.509 certificate hash, that will be used by the provisioned device
    /// @param deviceName Name the created device should have on the cloud,
    /// pass nullptr or an empty string if a random string should be used as a name instead
    Provision_Callback(X509_Certificate, function cb, const char *provisionDeviceKey, const char *provisionDeviceSecret, const char *hash, const char *deviceName = nullptr);

    /// @brief Gets the device profile provisioning key of the device profile,
    /// that should be used to create the device under
    /// @return Device profile provisioning key
    const char* Get_Device_Key() const;

    /// @brief Sets the device profile provisioning key of the device profile,
    /// that should be used to create the device under
    /// @param provisionDeviceKey Device profile provisioning key
    void Set_Device_Key(const char *provisionDeviceKey);

    /// @brief Gets the device profile provisioning secret of the device profile,
    /// that should be used to create the device under
    /// @return Device profile provisioning secret
    const char* Get_Device_Secret() const;

    /// @brief Gets the device profile provisioning secret of the device profile,
    /// that should be used to create the device under
    /// @param provisionDeviceSecret Device profile provisioning secret
    void Set_Device_Secret(const char *provisionDeviceSecret);

    /// @brief Gets the name the created device should have on the cloud,
    /// is a nullptr or an empty string if a random string should be used as a name instead
    /// @return Name the created device should have on the cloud
    const char* Get_Device_Name() const;

    /// @brief Sets the name the created device should have on the cloud,
    /// is a nullptr or an empty string if a random string should be used as a name instead
    /// @param deviceName Name the created device should have on the cloud
    void Set_Device_Name(const char *deviceName) ;

    /// @brief Gets the access token generated by the device,
    /// that will be used by the provisioned device,
    /// alternative to letting the access token be generated by the cloud instead
    /// @return Access token generated by the device
    const char* Get_Device_Access_Token() const;

    /// @brief Sets the access token generated by the device,
    /// that will be used by the provisioned device,
    /// alternative to letting the access token be generated by the cloud instead
    /// @param accessToken Access token generated by the device
    void Set_Device_Access_Token(const char *accessToken);

    /// @brief Gets the basic MQTT credentials username, that will be used by the provisioned device
    /// @return Basic MQTT credentials username
    const char* Get_Credentials_Username() const;

    /// @brief Sets the basic MQTT credentials username, that will be used by the provisioned device
    /// @param username Basic MQTT credentials username
    void Set_Credentials_Username(const char *username);

    /// @brief Gets the basic MQTT credentials password, that will be used by the provisioned device
    /// @return Basic MQTT credentials password
    const char* Get_Credentials_Password() const;

    /// @brief Sets the basic MQTT credentials password, that will be used by the provisioned device
    /// @param password Basic MQTT credentials password
    void Set_Credentials_Password(const char *password);

    /// @brief Gets the basic MQTT credentials clientID, that will be used by the provisioned device
    /// @return Basic MQTT credentials clientID
    const char* Get_Credentials_Client_ID() const;

    /// @brief Sets the basic MQTT credentials clientID, that will be used by the provisioned device
    /// @param clientID Basic MQTT credentials clientID
    void Set_Credentials_Client_ID(const char *clientID);

    /// @brief Gets the public X.509 certificate hash, that will be used by the provisioned device
    /// @return Public X.509 certificate hash
    const char* Get_Certificate_Hash() const;

    /// @brief Sets the public X.509 certificate hash, that will be used by the provisioned device
    /// @param hash Public X.509 certificate hash
    void Set_Certificate_Hash(const char *hash);

    /// @brief Gets the string containing the used credentials type that decides which provisioning method is actually used,
    /// by the Provision_Callback and therefore decides what response we will receive from the server
    /// @return String containing the used credentials type
    const char* Get_Credentials_Type() const;

  private:
    const char  *m_deviceKey;       // Device profile provisioning key
    const char  *m_deviceSecret;    // Device profile provisioning secret
    const char  *m_deviceName;      // Device name the provisioned device should have
    const char  *m_accessToken;     // Access token supplied by the device, if it should not be generated by the server instead
    const char  *m_credUsername;    // MQTT credential username, if the MQTT basic credentials method is used
    const char  *m_credPassword;    // MQTT credential password, if the MQTT basic credentials method is used
    const char  *m_credClientID;    // MQTT credential clientID, if Mthe QTT basic credentials method is used
    const char  *m_hash;            // X.509 certificate hash, if the X.509 certificate authentication method is used
    const char  *m_credentialsType; // Credentials type we are requesting from the server, nullptr for the default option (Credentials generated by the ThingsBoard server)
};

#endif // Provision_Callback_h
