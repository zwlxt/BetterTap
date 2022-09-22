#pragma once
#include <LittleFS.h>
#include <vector>
#include <ArduinoJson.h>
#include "Logging.h"

constexpr u8 JSON_SIZE = 0xFF;

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
    String initTopic;
    String v1Topic;
};

struct TimeConfig
{
    int tz = 8;
    int dst = 0;
    String ntpServer1 = "pool.ntp.org";
    String ntpServer2;
    String ntpServer3;
};

namespace app_config_converter_impl
{
    void decodeConfig(JsonObjectConst jsonObject, WiFiConfig &config);
    void encodeConfig(JsonObject jsonObject, const WiFiConfig &config);

    void decodeConfig(JsonObjectConst jsonObject, PinOutConfig &config);
    void encodeConfig(JsonObject jsonObject, const PinOutConfig &config);

    void decodeConfig(JsonObjectConst jsonObject, WebServerConfig &config);
    void encodeConfig(JsonObject jsonObject, const WebServerConfig &config);

    void decodeConfig(JsonObjectConst jsonObject, MQTTConfig &config);
    void encodeConfig(JsonObject jsonObject, const MQTTConfig &config);

    void decodeConfig(JsonObjectConst jsonObject, TimeConfig &config);
    void encodeConfig(JsonObject jsonObject, const TimeConfig &config);
}

template <typename T>
void loadConfig(File &file, T &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    DeserializationError ret = deserializeJson(document, file);
    JsonObjectConst obj = document.as<JsonObjectConst>();
    app_config_converter_impl::decodeConfig(obj, config);
}

template <typename T>
void saveConfig(File &file, const T &config)
{
    StaticJsonDocument<JSON_SIZE> document;
    JsonObject rootObj = document.to<JsonObject>();
    app_config_converter_impl::encodeConfig(rootObj, config);
    serializeJson(document, file);
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
    JsonArray array = document.to<JsonArray>();

    for (size_t i = 0; i < config.size(); ++i)
    {
        const T &cfgItem = config[i];
        JsonObject obj = array.createNestedObject();
        app_config_converter_impl::encodeConfig(obj, cfgItem);
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