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
    LOG_I("mounted ret=%c", fsMountRet ? 'T' : 'F');

    bool fsCheck = LittleFS.check();
    LOG_I("fsCheck=%c", fsCheck ? 'T' : 'F');
}

void setup()
{
    Serial.begin(115200);
    LOG_I("initializing");

    mountFS();

    networkManager.onConnected([&](auto &e) { app.init(); });
    networkManager.init();

    dashboard.init();
}

void loop()
{
    app.loop();
    dashboard.loop();
}
