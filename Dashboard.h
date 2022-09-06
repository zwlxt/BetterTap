#include "AppConfig.h"
#include "Constants.h"
#include "State.h"
#include <DNSServer.h>
#include <ESP8266WebServer.h>

class Dashboard
{
  public:
    template <typename T> using OnUpdateConfigHandler = std::function<void(const T &config)>;

    Dashboard() = default;

    void init();
    void loop();

    void onUpdateConfig(OnUpdateConfigHandler<std::vector<WiFiConfig>> handler)
    {
        m_updateSTAConfigHandler = handler;
    }

    void onUpdateConfig(OnUpdateConfigHandler<MQTTConfig> handler)
    {
        m_updateMQTTConfigHandler = handler;
    }

  private:
    ESP8266WebServer m_webServer;
    DNSServer m_dnsServer;

    OnUpdateConfigHandler<std::vector<WiFiConfig>> m_updateSTAConfigHandler;
    OnUpdateConfigHandler<MQTTConfig> m_updateMQTTConfigHandler;
};