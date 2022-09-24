#pragma once

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <map>
#include "AppConfig.h"
#include "State.h"
#include "Actuator.h"
#include "Protocol.h"
#include "Utils.h"

class App
{
public:
    App() : m_pubSubClient{m_wifiClient}, m_lastMQTTConnection{0} {}
    
    void init();
    void loop();
    void connectMQTT();

private:
    void setupActuators();
    void syncTime();
    void handleInitMessage(const u8 *payload, uint length);
    void handleMessage(const char *topic, const u8 *payload, uint length);
    void handleV1Message(const char* topic, const u8 *payload, uint length);

    template <typename T>
    void publishMessage(const char *topic, const T &payload, bool retain = false)
    {
        BytePrint out;
        protocolEncode(payload, out);

        m_pubSubClient.publish(topic, out.data(), out.size());
    }

    enum StateID
    {
        SYNC_CLOCK_COMPLETE = 0
    };

    State m_state;
    TimeConfig m_timeConfig;
    MQTTConfig m_mqttConfig;

    WiFiClient m_wifiClient;
    PubSubClient m_pubSubClient;
    protocol_v2::TopicConfig m_topics;

    unsigned long m_lastMQTTConnection;

    std::map<u8, TapActuator> m_tapActuators;
};
