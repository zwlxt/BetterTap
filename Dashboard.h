#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "Constants.h"
#include "AppConfig.h"

class Dashboard
{
  public:
    Dashboard() = default;

    void init();
    void loop();

  private:
    ESP8266WebServer m_webServer;
    DNSServer m_dnsServer;
};