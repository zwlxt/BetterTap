#include <ESP8266WebServer.h>
#include <DNSServer.h>

class Dashboard
{
public:
    Dashboard() = default;
    
    void init();
    void loop();
    
private:
    ESP8266WebServer m_webServer;
    DNSServer m_dnsServer;

    std::function<void()> handleGetPost(std::function<void()> getHandler, std::function<void()> postHandler);
};