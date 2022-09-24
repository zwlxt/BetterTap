#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <LittleFS.h>
#include "App.h"
#include "AppConfig.h"
#include "Logging.h"
#include "Network.h"
#include "Dashboard.h"

using namespace std;

NetworkManager networkManager;
Dashboard dashboard;
App app;

void mountFS()
{
    LOG_I("mounting fs");
    bool fsMountRet = LittleFS.begin();
    LOG_I("mounted ret=%d", fsMountRet);

    bool fsCheck = LittleFS.check();
    LOG_I("fsCheck=%d", fsCheck);
}

void setup()
{
    Serial.begin(115200);
    LOG_I("initializing");

    mountFS();

    networkManager.onConnected([&](auto &e) { app.init(); });
    networkManager.init();

    dashboard.onUpdateConfig([](const std::vector<WiFiConfig> &config) {
        LOG_I("updated STA config event");
        networkManager.init();
    });

    dashboard.onUpdateConfig([](const MQTTConfig &config) {
        LOG_I("updated MQTT config event");
        app.connectMQTT();
    });

    dashboard.init();
}

void loop()
{
    networkManager.loop();
    app.loop();
    dashboard.loop();
    yield();
}
