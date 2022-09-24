#include "AppConfig.h"

using namespace std;

namespace app_config_converter_impl
{
    void decodeConfig(JsonObjectConst document, WiFiConfig &config)
    {
        config.ssid = document["ssid"].as<String>();
        config.password = document["password"].as<String>();
    }

    void encodeConfig(JsonObject document, const WiFiConfig &config)
    {
        document["ssid"] = config.ssid;
        document["password"] = config.password;
    }

    void decodeConfig(JsonObjectConst document, PinOutConfig &config)
    {
        config.pin = document["pin"];
        config.en = document["en"];
    }

    void encodeConfig(JsonObject document, const PinOutConfig &config)
    {
        document["pin"] = config.pin;
        document["en"] = config.en;
    }

    void decodeConfig(JsonObjectConst document, WebServerConfig &config)
    {
        config.port = document["port"];
    }

    void encodeConfig(JsonObject document, const WebServerConfig &config)
    {
        document["port"] = config.port;
    }

    void decodeConfig(JsonObjectConst document, MQTTConfig &config)
    {
        config.host = document["host"].as<String>();
        config.port = document["port"];
        config.clientID = document["clientID"].as<String>();
        config.username = document["username"].as<String>();
        config.password = document["password"].as<String>();
        config.initTopic = document["initTopic"].as<String>();
        config.v1Topic = document["v1Topic"].as<String>();
    }

    void encodeConfig(JsonObject document, const MQTTConfig &config)
    {
        document["host"] = config.host;
        document["port"] = config.port;
        document["clientID"] = config.clientID;
        document["username"] = config.username;
        document["password"] = config.password;
        document["initTopic"] = config.initTopic;
        document["v1Topic"] = config.v1Topic;
    }

    void decodeConfig(JsonObjectConst document, TimeConfig &config)
    {
        config.tz = document["tz"];
        config.dst = document["dst"];
        config.ntpServer1 = document["ntp1"].as<String>();
        config.ntpServer2 = document["ntp2"].as<String>();
        config.ntpServer3 = document["ntp3"].as<String>();
    }

    void encodeConfig(JsonObject document, const TimeConfig &config)
    {
        document["tz"] = config.tz;
        document["dst"] = config.dst;
        document["ntp1"] = config.ntpServer1;
        document["ntp2"] = config.ntpServer2;
        document["ntp3"] = config.ntpServer3;
    }
}