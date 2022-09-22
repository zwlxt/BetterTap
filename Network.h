#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "State.h"

class NetworkManager
{
public:
    NetworkManager() {}

    void init();
    void loop();
    void onConnected(std::function<void(const WiFiEventStationModeGotIP &)> handler);
    
private:
    void prepareNetworkConfig();
    void stopNetworkConfig();
    void handleOnGotIP(const WiFiEventStationModeGotIP &e);
    void handleOnSTADisconnect(const WiFiEventStationModeDisconnected &e);

    enum NetworkState
    {
        CONFIGURING_WIFI = 1,
    };

    ESP8266WiFiMulti m_wifiMulti;
    WiFiEventHandler m_gotIPHandler;
    std::function<void(const WiFiEventStationModeGotIP &)> m_onConnected;
    State m_state;
};
