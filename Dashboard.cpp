#include "Dashboard.h"
#include "Assets.h"
#include "Logging.h"

void Dashboard::init()
{
    LOG_I("initializing dashboard");

    m_dnsServer.setErrorReplyCode(DNSReplyCode::NonExistentDomain);
    m_dnsServer.start(53, "tap2.io", WiFi.softAPIP());

    m_webServer.on("/", [=] {
        m_webServer.sendHeader("Location", "/dashboard");
        m_webServer.send(302, "text/plan", "");
    });

    m_webServer.on("/dashboard",
                   [=] { m_webServer.send_P(200, "text/html", ASSET_DASHBOARD_HTML, ASSET_DASHBOARD_HTML_LEN); });

    m_webServer.on("/css/bulma.min.css", [=] {
        m_webServer.sendHeader("Cache-Control", "max-age=3600");
        m_webServer.send_P(200, "text/css", ASSET_BULMA_MIN_CSS, ASSET_BULMA_MIN_CSS_LEN);
    });

    m_webServer.on("/dashboard/wifi", handleGetPost([=] {  },
                                                    [=] {

                                                    }));

    m_webServer.begin(80);
}

void Dashboard::loop()
{
    m_webServer.handleClient();
    m_dnsServer.processNextRequest();
}

std::function<void()> Dashboard::handleGetPost(std::function<void()> getHandler, std::function<void()> postHandler)
{
    return [=] {
        if (m_webServer.method() == HTTP_GET)
        {
            getHandler();
        }
        else if (m_webServer.method() == HTTP_POST)
        {
            postHandler();
        }
        else
        {
            m_webServer.send(405, "text/plain", "");
        }
    };
}
