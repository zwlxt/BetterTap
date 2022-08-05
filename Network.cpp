#include "Constants.h"
#include "Network.h"
#include "AppConfig.h"
#include "StreamUtils.h"

#define DEFAULT_AP_PSK "v5qCQ1sr"

void NetworkManager::init()
{
    std::vector<WiFiConfig> wifiConfig;
    loadOrSaveConfig(STA_CONFIG, wifiConfig);

    if (!wifiConfig.empty())
    {
        prepareNetworkConfig();
    }

    WiFi.mode(WIFI_STA);
    m_gotIPHandler = WiFi.onStationModeGotIP([=](auto e)
                                             { this->onGotIP(e); });

    m_wifiMulti.cleanAPlist();

    for (const auto &cfg : wifiConfig)
    {
        m_wifiMulti.addAP(cfg.ssid.c_str(), cfg.password.c_str());
    }
}

void NetworkManager::updateState()
{
    if (m_wifiMulti.run(5000) != WL_CONNECTED)
    {
        prepareNetworkConfig();
    }
}

void NetworkManager::prepareNetworkConfig()
{
    if (WiFi.getMode() != WIFI_AP_STA && WiFi.getMode() != WIFI_AP)
    {
        WiFi.mode(WIFI_AP_STA);

        String ssid = String(PSTR("TAP2_")) + String(ESP.getChipId(), HEX);
        LOG_I("starting AP: %d", ssid.c_str());
        WiFi.softAP(ssid, DEFAULT_AP_PSK);
        LOG_I("started AP");
    }

    LOG_I("starting smart config");
    WiFi.beginSmartConfig();
}

void NetworkManager::stopNetworkConfig()
{
    LOG_I("switching to STA mode");
    WiFi.mode(WIFI_STA);

    LOG_I("stopping smart config");
    WiFi.stopSmartConfig();
}

void NetworkManager::onGotIP(const WiFiEventStationModeGotIP &e)
{
    StringPrint ipEvent;
    ipEvent.print("IP:");
    ipEvent.print(e.ip);
    ipEvent.print(" GW:");
    ipEvent.print(e.gw);
    ipEvent.print(" MASK:");
    ipEvent.print(e.mask);
    String ipEventDesc = ipEvent.str();

    LOG_I("got IP address: %s", ipEventDesc.c_str());
    stopNetworkConfig();
}
