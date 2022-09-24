#include "Network.h"
#include "AppConfig.h"
#include "Constants.h"
#include "Utils.h"

#define DEFAULT_AP_PSK "v5qCQ1sr"

void NetworkManager::init()
{
    LOG_I("loading config from %s", STA_CONFIG);

    std::vector<WiFiConfig> wifiConfig;
    loadOrSaveConfig_P(STA_CONFIG, wifiConfig);

    if (wifiConfig.empty())
    {
        prepareNetworkConfig();
    }

    m_gotIPHandler = WiFi.onStationModeGotIP([=](auto e) {
        this->handleOnGotIP(e);
        
        if (m_onConnected)
        {
            m_onConnected(e);
        }
    });

    WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected &) {
        
    });

    m_wifiMulti.cleanAPlist();

    for (const auto &cfg : wifiConfig)
    {
        LOG_I("adding AP: %s, psk:%s", cfg.ssid.c_str(), cfg.password.isEmpty() ? "" : "*");
        m_wifiMulti.addAP(cfg.ssid.c_str(), cfg.password.c_str());
    }

    m_wifiMulti.run();
}

void NetworkManager::loop()
{
    if (!WiFi.isConnected() && m_state[CONFIGURING_WIFI])
    {
        prepareNetworkConfig();
    }
}

void NetworkManager::onConnected(std::function<void(const WiFiEventStationModeGotIP &)> handler)
{
    this->m_onConnected = handler;
}

void NetworkManager::prepareNetworkConfig()
{
    if (WiFi.getMode() != WIFI_AP_STA && WiFi.getMode() != WIFI_AP)
    {
        WiFi.mode(WIFI_AP_STA);

        String ssid = String(PSTR("TAP2_")) + String(ESP.getChipId(), HEX);
        LOG_I("starting AP: %s", ssid.c_str());
        bool ret = WiFi.softAP(ssid, DEFAULT_AP_PSK, 6, false, 4);
        LOG_I("started AP ret=%c", ret ? 'T' : 'F');

        if (ret)
        {
            BytePrint ipBuffer;
            ipBuffer.print(WiFi.softAPIP());
            LOG_I("IP=%s", ipBuffer.to_str());
        }

        LOG_I("starting smart config");
        WiFi.beginSmartConfig();

        m_state.set(CONFIGURING_WIFI);
    }
}

void NetworkManager::stopNetworkConfig()
{
    LOG_I("switching to STA mode");
    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_STA);

    LOG_I("stopping smart config");
    WiFi.stopSmartConfig();

    m_state.unset(CONFIGURING_WIFI);
}

void NetworkManager::handleOnGotIP(const WiFiEventStationModeGotIP &e)
{
    BytePrint ipEvent;
    ipEvent.print("IP:");
    ipEvent.print(e.ip);
    ipEvent.print(" GW:");
    ipEvent.print(e.gw);
    ipEvent.print(" MASK:");
    ipEvent.print(e.mask);

    LOG_I("got IP address: %s", ipEvent.to_str());
    stopNetworkConfig();
}
