#pragma once
#include <LittleFS.h>
#include <vector>
#include <ArduinoJson.h>
#include "Logging.h"

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
    String clientID;
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
    void decodeConfig(JsonObjectConst document, WiFiConfig &config);
    void encodeConfig(JsonObject document, const WiFiConfig &config);

    void decodeConfig(JsonObjectConst document, PinOutConfig &config);
    void encodeConfig(JsonObject document, const PinOutConfig &config);

    void decodeConfig(JsonObjectConst document, WebServerConfig &config);
    void encodeConfig(JsonObject document, const WebServerConfig &config);

    void decodeConfig(JsonObjectConst document, MQTTConfig &config);
    void encodeConfig(JsonObject document, const MQTTConfig &config);

    void decodeConfig(JsonObjectConst document, TimeConfig &config);
    void encodeConfig(JsonObject document, const TimeConfig &config);

    void decodeConfig(JsonObjectConst document, TopicConfig &config);
    void encodeConfig(JsonObject document, const TopicConfig &config);
}

template <typename T>
void loadConfig(File &file, T &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    DeserializationError ret = deserializeJson(document, file);

    app_config_converter_impl::decodeConfig(document.as<JsonObjectConst>(), config);
}

template <typename T>
void saveConfig(File &file, const T &config)
{
    StaticJsonDocument<JSON_SIZE> document;

    app_config_converter_impl::encodeConfig(document.to<JsonObject>(), config);
    serializeJsonPretty(document, file);
}

template <typename T>
void loadConfig(File &file, std::vector<T> &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    DeserializationError ret = deserializeJson(document, file);

    JsonArrayConst jsonArray = document.as<JsonArrayConst>();

    for (JsonObjectConst item : jsonArray)
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
        const T &cfgItem = config[i];
        app_config_converter_impl::encodeConfig(document[i], cfgItem);
    }

    serializeJson(document, file);
}

template <typename T>
void loadOrSaveConfig(const char *path, T &config)
{
    LOG_I("loading config %s", path);
    if (LittleFS.exists(path))
    {
        File file = LittleFS.open(path, "r");
        loadConfig(file, config);
    }
    else
    {
        LOG_I("config file %s does not exist, creating", path);
        File file = LittleFS.open(path, "w");
        saveConfig(file, config);
    }
}