#include "Dashboard.h"
#include "Assets.h"
#include "Logging.h"
#include "Constants.h"
#include <uri/UriRegex.h>

void Dashboard::init()
{
    LOG_I("initializing dashboard");

    m_dnsServer.setErrorReplyCode(DNSReplyCode::NonExistentDomain);
    m_dnsServer.start(53, "tap2.io", WiFi.softAPIP());

    m_webServer.on("/", [=] {
        m_webServer.sendHeader("Location", "/dashboard");
        m_webServer.send(302);
    });

    m_webServer.on("/dashboard",
                   [=] { m_webServer.send_P(200, MIME_HTML, ASSET_DASHBOARD_HTML, ASSET_DASHBOARD_HTML_LEN); });

    m_webServer.on("/css/style.css", [=] {
        m_webServer.send_P(200, MIME_CSS, ASSET_STYLE_CSS, ASSET_STYLE_CSS_LEN);
    });

    m_webServer.on("/dashboard/wifi", [=] {
        std::vector<WiFiConfig> config;
        loadOrSaveConfig_P(STA_CONFIG, config);

        String currentSSID = WiFi.SSID();

        String delFormTemplate = F("<form action=\"/cfg/wifi/del\" method=\"post\">"
                                   "<input type=\"hidden\" name=\"ssid\" value=\"{{SSID}}\"/>"
                                   "<button>Delete</button>"
                                   "</form>");

        String tableItemHTML;
        for (const auto &cfg : config)
        {
            String delForm = delFormTemplate;
            delForm.replace("{{SSID}}", cfg.ssid);

            tableItemHTML += "<tr>";
            tableItemHTML += "<td>";
            tableItemHTML += cfg.ssid == currentSSID ? "*" : "-";
            tableItemHTML += "</td>";
            tableItemHTML += "<td>" + cfg.ssid + "</td>";
            tableItemHTML += "<td>" + delForm + "</td>";
            tableItemHTML += "</tr>";
        }

        if (tableItemHTML.isEmpty())
        {
            tableItemHTML = "<tr><td></td><td>(Empty)</td><td></td></tr>";
        }

        String html(FPSTR(ASSET_DASHBOARD_WIFI_HTML));

        html.replace(F("{{CFG_ITEMS}}"), tableItemHTML);

        m_webServer.send(200, MIME_HTML, html);
    });

    m_webServer.on("/cfg/wifi/add", [=] {
        LOG_D("args: %s", m_webServer.arg("plain").c_str());

        if (m_webServer.hasArg(F("ssid")))
        {
            String ssid = m_webServer.arg(F("ssid"));

            String password;
            if (m_webServer.hasArg(F("password")))
            {
                password = m_webServer.arg(F("password"));
            }

            std::vector<WiFiConfig> config;
            loadOrSaveConfig_P(STA_CONFIG, config);
            config.emplace_back(WiFiConfig{ssid, password});
            LOG_D("sizeof cfg %d", config.size());
            saveConfig_P(STA_CONFIG, config);

            m_updateSTAConfigHandler(config);
        }

        m_webServer.send(200);
    });

    m_webServer.on("/cfg/wifi/del", [=] {
        if (m_webServer.hasArg(F("ssid")))
        {
            String ssid = m_webServer.arg(F("ssid"));

            std::vector<WiFiConfig> config;
            loadOrSaveConfig_P(STA_CONFIG, config);

            for (const auto &cfg : config)
            {
                config.erase(
                    std::remove_if(config.begin(), config.end(), [&](const WiFiConfig &x) { return ssid == x.ssid; }));
            }

            saveConfig_P(STA_CONFIG, config);

            m_updateSTAConfigHandler(config);
        }

        m_webServer.send(200);
    });

    m_webServer.on("/dashboard/server", [=] {
        MQTTConfig config;
        loadOrSaveConfig_P(MQTT_CONFIG_FILE, config);

        String html(FPSTR(ASSET_DASHBOARD_SERVER_HTML));
        html.replace(F("{{HOST}}"), config.host);
        html.replace(F("{{PORT}}"), String(config.port));
        html.replace(F("{{CLIENT_ID}}"), config.clientID);
        html.replace(F("{{USERNAME}}"), config.username);
        html.replace(F("{{PASSWORD}}"), config.password);
        
        m_webServer.send(200, MIME_HTML, html);
    });

    m_webServer.on("/cfg/mqtt", [=] {

        if (m_webServer.hasArg("host") && m_webServer.hasArg("port") && m_webServer.hasArg("clientID") &&
            m_webServer.hasArg("username") && m_webServer.hasArg("password"))
        {
            MQTTConfig config;
            config.host = m_webServer.arg("host");
            config.port = m_webServer.arg("port").toInt();
            config.clientID = m_webServer.arg("clientID");
            config.username = m_webServer.arg("username");
            config.password = m_webServer.arg("password");

            saveConfig_P(MQTT_CONFIG_FILE, config);

            m_updateMQTTConfigHandler(config);
        }

        m_webServer.send(200);
    });

    m_webServer.on(UriRegex("/dbg/fs/(get|rm)/(.+)"), [=] {
        String action = m_webServer.pathArg(0);
        String filePath = m_webServer.pathArg(1);

        if (!filePath.isEmpty())
        {
            filePath = "/" + filePath;

            LOG_I("filePath=%s", filePath.c_str());

            if (LittleFS.exists(filePath))
            {
                if (action == "get")
                {
                    File file = LittleFS.open(filePath, "r");
                    if (file.isFile())
                    {
                        m_webServer.streamFile(file, "text/plain");
                        return;
                    }
                }
                else if (action == "rm")
                {
                    LittleFS.remove(filePath);
                    m_webServer.send(200);
                    return;
                }
            }
        }

        m_webServer.send(404);
    });

    m_webServer.addHook([=](const String &method, const String &url, WiFiClient *client,
                            ESP8266WebServer::ContentTypeFunction contentType) {
        if (url.startsWith("/cfg"))
        {
            if (method == "POST")
            {
                return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
            }
            else
            {
                m_webServer.send(405);
                return ESP8266WebServer::CLIENT_REQUEST_IS_HANDLED;
            }
        }
        else
        {
            if (method == "GET")
            {
                return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
            }
            else
            {
                m_webServer.send(405);
                return ESP8266WebServer::CLIENT_REQUEST_IS_HANDLED;
            }
        }
    });

    m_webServer.begin(80);
}

void Dashboard::loop()
{
    m_webServer.handleClient();
    m_dnsServer.processNextRequest();
}
