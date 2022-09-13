#include "AppConfig.h"

using namespace std;

namespace app_config_converter_impl
{
    void decodeConfig(JsonVariantConst document, WiFiConfig &config)
    {
        config.ssid = document["ssid"].as<String>();
        config.password = document["password"].as<String>();
    }

    void encodeConfig(JsonVariant document, const WiFiConfig &config)
    {
        document["ssid"] = config.ssid;
        document["password"] = config.password;
    }

    void decodeConfig(JsonVariantConst document, PinOutConfig &config)
    {
        config.id = document["id"];
        config.pin = document["pin"];
        config.en = document["en"];
    }

    void encodeConfig(JsonVariant document, const PinOutConfig &config)
    {
        document["id"] = config.id;
        document["pin"] = config.pin;
        document["en"] = config.en;
    }

    void decodeConfig(JsonVariantConst document, WebServerConfig &config)
    {
        config.port = document["port"];
    }

    void encodeConfig(JsonVariant document, const WebServerConfig &config)
    {
        document["port"] = config.port;
    }

    void decodeConfig(JsonVariantConst document, MQTTConfig &config)
    {
        config.host = document["host"].as<String>();
        config.port = document["port"];
        config.clientID = document["clientID"].as<String>();
        config.username = document["username"].as<String>();
        config.password = document["password"].as<String>();
    }

    void encodeConfig(JsonVariant document, const MQTTConfig &config)
    {
        document["host"] = config.host;
        document["port"] = config.port;
        document["clientID"] = config.clientID;
        document["username"] = config.username;
        document["password"] = config.password;
    }

    void decodeConfig(JsonVariantConst document, TimeConfig &config)
    {
        config.tz = document["tz"];
        config.dst = document["dst"];
        config.ntpServer1 = document["ntp1"].as<String>();
        config.ntpServer2 = document["ntp2"].as<String>();
        config.ntpServer3 = document["ntp3"].as<String>();
    }

    void encodeConfig(JsonVariant document, const TimeConfig &config)
    {
        document["tz"] = config.tz;
        document["dst"] = config.dst;
        document["ntp1"] = config.ntpServer1;
        document["ntp2"] = config.ntpServer2;
        document["ntp3"] = config.ntpServer3;
    }
}