#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "AppConfig.h"
#include "Logging.h"
#include "App.h"

using namespace std;

App app;

void mountFS()
{
    LittleFS.setConfig(LittleFSConfig(true));

    LOG_I("mounting fs");
    bool fsMountRet = LittleFS.begin();
    LOG_I("mounted ret=%d", fsMountRet);
}

void setupPins()
{
    LOG_I("initializing PINs");
    
    vector<PinOutConfig> pinOutConfig;
    loadOrSaveConfig("/config/pinout.config", pinOutConfig);

    LOG_I("loaded %d pin configurations", pinOutConfig.size());
    
    for (const auto &cfg : pinOutConfig)
    {
        pinMode(cfg.pin, OUTPUT);
    }

    LOG_I("completed");
}

void setup()
{
    Serial.begin(115200);
    LOG_I("initializing");

    mountFS();

    app.init();
}

void loop()
{
    app.loop();
}
