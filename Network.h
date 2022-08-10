#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

class NetworkManager
{
public:
    NetworkManager() {}

    void init();
    void updateState();
    void onConnected(std::function<void(const WiFiEventStationModeGotIP &)> handler);
    
private:
    void prepareNetworkConfig();
    void stopNetworkConfig();
    void onGotIP(const WiFiEventStationModeGotIP &e);

    ESP8266WiFiMulti m_wifiMulti;
    WiFiEventHandler m_gotIPHandler;
    std::function<void(const WiFiEventStationModeGotIP &)> m_onConnected;
};
