#pragma once

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <coredecls.h>
#include "AppConfig.h"
#include "State.h"
#include "Logging.h"

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

template <typename T>
static void loadOrSaveConfig(const char *path, T &config)
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
