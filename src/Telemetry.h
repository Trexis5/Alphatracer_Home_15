/*
  Telemetry.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Telemetry_h
#define Telemetry_h

// Local includes.
#include "Configuration.h"

// Library includes.
#include <ArduinoJson.h>
#if THINGSBOARD_ENABLE_STL
#include <type_traits>
#endif // THINGSBOARD_ENABLE_STL

/// @brief Telemetry record class, allows to store different data using a common interface.
class Telemetry {
  public:
    /// @brief Creates an empty Telemetry record containg neither a key nor value
    Telemetry();

    /// @brief Constructs telemetry record from integral value
    /// @tparam T Type of the passed value, is required to be integral,
    /// to ensure this constructor isn't used instead of the float one by mistake
    /// @param key Key of the key value pair we want to create
    /// @param value Value of the key value pair we want to create
    template <typename T,
#if THINGSBOARD_ENABLE_STL
              // Standard library is_integral, includes bool, char, signed char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, long long, and unsigned long long
              typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
#else
              // Workaround for ArduinoJson version after 6.21.0, to still be able to access internal enable_if and is_integral declarations, previously accessible with ARDUINOJSON_NAMESPACE
              typename ArduinoJson::ARDUINOJSON_VERSION_NAMESPACE::detail::enable_if<ArduinoJson::ARDUINOJSON_VERSION_NAMESPACE::detail::is_integral<T>::value>::type* = nullptr>
#endif // THINGSBOARD_ENABLE_STL
    inline Telemetry(const char *key, T value)
      : m_type(DataType::TYPE_INT),
      m_key(key),
      m_value()
    {
        m_value.integer = value;
    }

    /// @brief Constructs telemetry record from floating point value
    /// @tparam T Type of the passed value, is required to be a floating point,
    /// to ensure this constructor isn't used instead of the boolean one by mistake
    /// @param key Key of the key value pair we want to create
    /// @param value Value of the key value pair we want to create
    template <typename T,
#if THINGSBOARD_ENABLE_STL
              // Standard library is_floating_point, includes float and double
              typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
#else
              // Workaround for ArduinoJson version after 6.21.0, to still be able to access internal enable_if and is_floating_point declarations, previously accessible with ARDUINOJSON_NAMESPACE
              typename ArduinoJson::ARDUINOJSON_VERSION_NAMESPACE::detail::enable_if<ArduinoJson::ARDUINOJSON_VERSION_NAMESPACE::detail::is_floating_point<T>::value>::type* = nullptr>
#endif // THINGSBOARD_ENABLE_STL
    Telemetry(const char *key, T value)
      : m_type(DataType::TYPE_REAL),
      m_key(key),
      m_value()
    {
      m_value.real = value;
    }

    /// @brief Constructs telemetry record from boolean value	
    /// @param key Key of the key value pair we want to create	
    /// @param val Value of the key value pair we want to create	
    Telemetry(const char *key, bool val);

    /// @brief Constructs telemetry record from string value
    /// @param key Key of the key value pair we want to create
    /// @param value Value of the key value pair we want to create
    Telemetry(const char *key, const char *value);

    /// @brief Whether this record is empty or not
    /// @return Whether there is any data in this record or not
    bool IsEmpty() const;

    /// @brief Serializes the key-value pair depending on the constructor used
    /// @param jsonObj Object the value will be copied into with the given key
    /// @return Whether serializing was successfull or not
    bool SerializeKeyValue(const JsonVariant &jsonObj) const;

  private:
    // Data container
    union Data {
        const char  *str;
        bool        boolean;
        int64_t     integer;
        double      real;
    };

    // Data type inside a container
    enum class DataType: const uint8_t {
        TYPE_NONE,
        TYPE_BOOL,
        TYPE_INT,
        TYPE_REAL,
        TYPE_STR
    };

    DataType     m_type;  // Data type flag
    const char   *m_key;  // Data key
    Data         m_value; // Data value
};

// Convenient aliases
using Attribute = Telemetry;

#endif // Telemetry_h
