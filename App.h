#pragma once

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <StreamUtils.h>
#include <map>
#include "AppConfig.h"
#include "State.h"
#include "Actuator.h"
#include "Protocol.h"

class App
{
public:
    void init();
    void loop();

private:
    void setupActuators();
    void syncTime();
    void connectMQTT();
    void handleMessage(const char *topic, const u8 *payload, uint length, const TopicConfig &topics);

    template <typename T>
    void responseMessage(const char *topic, const T &payload, bool retain = false)
    {
        StringPrint out;
        protocolEncode(payload, out);

        String str = out.str();
        m_pubSubClient.publish(topic, str.c_str(), str.length());
    }

    enum StateID
    {
        syncClockComplete = 0
    };

    State<8> m_state;

    WiFiClientSecure m_wifiClient;
    PubSubClient m_pubSubClient;

    std::map<u8, TapActuator> m_tapActuators;
};
