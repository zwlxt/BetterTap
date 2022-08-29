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
    u8 id;
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
    int tz = 8;
    int dst = 0;
    String ntpServer1 = "pool.ntp.org";
    String ntpServer2;
    String ntpServer3;
};

struct TopicConfig
{
    String legacyAction;
};

namespace app_config_converter_impl
{
    void decodeConfig(JsonVariantConst document, WiFiConfig &config);
    void encodeConfig(JsonVariant document, const WiFiConfig &config);

    void decodeConfig(JsonVariantConst document, PinOutConfig &config);
    void encodeConfig(JsonVariant document, const PinOutConfig &config);

    void decodeConfig(JsonVariantConst document, WebServerConfig &config);
    void encodeConfig(JsonVariant document, const WebServerConfig &config);

    void decodeConfig(JsonVariantConst document, MQTTConfig &config);
    void encodeConfig(JsonVariant document, const MQTTConfig &config);

    void decodeConfig(JsonVariantConst document, TimeConfig &config);
    void encodeConfig(JsonVariant document, const TimeConfig &config);

    void decodeConfig(JsonVariantConst document, TopicConfig &config);
    void encodeConfig(JsonVariant document, const TopicConfig &config);
}

template <typename T>
void decodeConfig(JsonVariantConst document, T &config)
{
    app_config_converter_impl::decodeConfig(document, config);
}

template <typename T>
void encodeConfig(JsonVariant document, const T &config)
{
    app_config_converter_impl::encodeConfig(document, config);
}

template <typename T>
void loadConfig(File &file, T &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    DeserializationError ret = deserializeJson(document, file);

    decodeConfig(document.as<JsonVariantConst>(), config);
}

template <typename T>
void saveConfig(File &file, const T &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    
    encodeConfig(document.as<JsonObject>(), config);
    serializeJsonPretty(document, file);
}

template <typename T>
void loadConfig(File &file, std::vector<T> &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    DeserializationError ret = deserializeJson(document, file);

    JsonArrayConst jsonArray = document.as<JsonArrayConst>();

    for (JsonVariantConst item : jsonArray)
    {
        T cfgItem;
        decodeConfig(item, cfgItem);
        config.push_back(cfgItem);
    }
}

template <typename T>
void saveConfig(File &file, const std::vector<T> &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    JsonArray array = document.to<JsonArray>();

    for (size_t i = 0; i < config.size(); ++i)
    {
        const T &cfgItem = config[i];
        JsonObject obj = array.createNestedObject();
        encodeConfig(obj, cfgItem);
    }

    serializeJson(document, file);
}

template <typename T>
void loadOrSaveConfig_P(const char *path_P, T &config)
{
    String path(FPSTR(path_P));
    if (LittleFS.exists(path))
    {
        LOG_I("loading config %s", path.c_str());
        File file = LittleFS.open(path, "r");
        loadConfig(file, config);
    }
    else
    {
        LOG_I("config file %s does not exist, creating", path.c_str());
        File file = LittleFS.open(path, "w");
        saveConfig(file, config);
    }
    LOG_I("done");
}

template <typename T>
void saveConfig_P(const char *path_P, T &config)
{
    String path(FPSTR(path_P));
    LOG_I("saving config file %s", path.c_str());
    File file = LittleFS.open(path, "w+");
    saveConfig(file, config);
    LOG_I("done");
}