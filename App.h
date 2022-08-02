#pragma once

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "AppConfig.h"
#include "State.h"

class App
{
public:
    void init();
    void loop();

private:
    void syncTime();
    void connectMQTT();
    void handleMessage(const char *topic, const u8 *payload, uint length, const TopicConfig &topics);

    enum StateID
    {
        syncClockComplete = 0
    };

    State<8> m_state;

    WiFiClientSecure m_wifiClient;
    PubSubClient m_pubSubClient;
};
