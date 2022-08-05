#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

class NetworkManager
{
public:
    NetworkManager() {}

    void init();
    void updateState();
private:
    void prepareNetworkConfig();
    void stopNetworkConfig();
    void onGotIP(const WiFiEventStationModeGotIP &e);

    ESP8266WiFiMulti m_wifiMulti;
    WiFiEventHandler m_gotIPHandler;
};
