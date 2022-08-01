#include "AppConfig.h"

using namespace std;

namespace app_config_converter_impl
{
    void decodeConfig(const JsonDocument &document, WiFiConfig &config)
    {
        config.ssid = document["ssid"].as<String>();
        config.password = document["password"].as<String>();
    }

    void encodeConfig(JsonDocument &document, const WiFiConfig &config)
    {
        document["ssid"] = config.ssid;
        document["password"] = config.password;
    }

    void decodeConfig(const JsonDocument &document, PinOutConfig &config)
    {
        config.pin = document["pin"];
        config.en = document["en"];
    }

    void encodeConfig(JsonDocument &document, const PinOutConfig &config)
    {
        document["pin"] = config.pin;
        document["en"] = config.en;
    }

    void decodeConfig(const JsonDocument &document, WebServerConfig &config)
    {
        config.port = document["port"];
    }

    void encodeConfig(JsonDocument &document, const WebServerConfig &config)
    {
        document["port"] = config.port;
    }

    void decodeConfig(const JsonDocument &document, MQTTConfig &config)
    {
        config.host = document["host"].as<String>();
        config.port = document["port"];
    }

    void encodeConfig(JsonDocument &document, const MQTTConfig &config)
    {
        document["host"] = config.host;
        document["port"] = config.port;
    }

    void decodeConfig(const JsonDocument &document, TimeConfig &config)
    {
        config.tz = document["tz"];
        config.dst = document["dst"];
        config.ntpServer1 = document["ntp1"].as<String>();
        config.ntpServer2 = document["ntp2"].as<String>();
        config.ntpServer3 = document["ntp3"].as<String>();
    }

    void encodeConfig(JsonDocument &document, const TimeConfig &config)
    {
        document["tz"] = config.tz;
        document["dst"] = config.dst;
        document["ntp1"] = config.ntpServer1;
        document["ntp2"] = config.ntpServer2;
        document["ntp3"] = config.ntpServer3;
    }

    void decodeConfig(const JsonDocument &document, TopicConfig &config)
    {
        config.action = document["action"].as<String>();
    }

    void encodeConfig(JsonDocument &document, const TopicConfig &config)
    {
        document["action"] = config.action;
    }
}