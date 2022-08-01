#pragma once
#include <LittleFS.h>
#include <vector>
#include <ArduinoJson.h>
#include <TZ.h>

constexpr u8 JSON_SIZE = 128;

struct WiFiConfig
{
    String ssid;
    String password;
};

struct PinOutConfig
{
    u8 pin;
    u8 en;
};

struct WebServerConfig
{
    u16 port = 80;
};

struct MQTTConfig
{
    String host;
    u16 port;
    String username;
    String password;
};

struct TimeConfig
{
    int tz;
    int dst;
    String ntpServer1;
    String ntpServer2;
    String ntpServer3;
};

struct TopicConfig
{
    String action;
};

namespace app_config_converter_impl
{
    void decodeConfig(const JsonDocument &document, WiFiConfig &config);
    void encodeConfig(JsonDocument &document, const WiFiConfig &config);

    void decodeConfig(const JsonDocument &document, PinOutConfig &config);
    void encodeConfig(JsonDocument &document, const PinOutConfig &config);

    void decodeConfig(const JsonDocument &document, WebServerConfig &config);
    void encodeConfig(JsonDocument &document, const WebServerConfig &config);

    void decodeConfig(const JsonDocument &document, MQTTConfig &config);
    void encodeConfig(JsonDocument &document, const MQTTConfig &config);

    void decodeConfig(const JsonDocument &document, TimeConfig &config);
    void encodeConfig(JsonDocument &document, const TimeConfig &config);

    void decodeConfig(const JsonDocument &document, TopicConfig &config);
    void encodeConfig(JsonDocument &document, const TopicConfig &config);
}

template <typename T>
void loadConfig(File &file, T &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    DeserializationError ret = deserializeJson(document, file);

    app_config_converter_impl::decodeConfig(document, config);
}

template <typename T>
void saveConfig(File &file, const T &config)
{
    StaticJsonDocument<JSON_SIZE> document;

    app_config_converter_impl::encodeConfig(document, config);
    serializeJsonPretty(document, file);
}

template <typename T>
void loadConfig(File &file, std::vector<T> &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    DeserializationError ret = deserializeJson(document, file);

    for (const JsonDocument &item : document.as<JsonArrayConst>)
    {
        T cfgItem;
        app_config_converter_impl::decodeConfig(item, cfgItem);
        config.push_back(cfgItem);
    }
}

template <typename T>
void saveConfig(File &file, const std::vector<T> &config)
{
    StaticJsonDocument<JSON_SIZE> document;

    for (size_t i = 0; i < config.size(); ++i)
    {
        T &cfgItem = config[i];
        JsonDocument &docItem = document[i];
        app_config_converter_impl::encodeConfig(docItem, cfgItem);
    }

    serializeJson(document, file);
}
