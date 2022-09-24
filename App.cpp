#include "App.h"
#include "AppConfig.h"
#include "Constants.h"
#include "Logging.h"
#include <coredecls.h>
#include <vector>

using namespace protocol_onenet;
using namespace protocol_v1;

void App::init()
{
    LOG_I("initializing app");

    setupActuators();
    syncTime();

    LOG_I("done");
}

void App::loop()
{
    if (m_state[SYNC_CLOCK_COMPLETE] && !m_pubSubClient.connected())
    {
        if (millis() - m_lastMQTTConnection > 5000)
        {
            connectMQTT();
        }
    }
    else
    {
        m_pubSubClient.loop();
    }
}

void App::setupActuators()
{
    LOG_I("initializing PINs");

    m_tapActuators.emplace(V1_PIN, PinOutConfig{V1_PIN, 1});

    std::vector<PinOutConfig> pinOutConfig;
    loadOrSaveConfig_P(PIN_OUT_CONFIG, pinOutConfig);

    for (auto cfg : pinOutConfig)
    {
        m_tapActuators.emplace(cfg.pin, cfg);
    }

    LOG_I("loaded %d pin configurations", m_tapActuators.size());
    LOG_I("completed");
}

void App::syncTime()
{
    LOG_I("starting");

    if (m_timeConfig.ntpServer1.isEmpty())
    {
        LOG_E("ntpServer1 is empty");
    }

    const char *ntpServer1 = m_timeConfig.ntpServer1.c_str();
    const char *ntpServer2 = m_timeConfig.ntpServer2.isEmpty() ? nullptr : m_timeConfig.ntpServer2.c_str();
    const char *ntpServer3 = m_timeConfig.ntpServer3.isEmpty() ? nullptr : m_timeConfig.ntpServer3.c_str();

    settimeofday_cb([=](bool fromSNTP) {
        LOG_I("sync time success, fromSNTP=%d", fromSNTP);
        m_state.set(StateID::SYNC_CLOCK_COMPLETE);

        connectMQTT();
    });

    if (ntpServer1 != nullptr)
    {
        LOG_I("configuring time with servers %s", ntpServer1);
    }
    if (ntpServer2 != nullptr)
    {
        LOG_I("configuring time with servers %s", ntpServer2);
    }
    if (ntpServer3 != nullptr)
    {
        LOG_I("configuring time with servers %s", ntpServer3);
    }

    configTime(m_timeConfig.tz, m_timeConfig.dst, ntpServer1, ntpServer2, ntpServer3);
}

void App::connectMQTT()
{
    LOG_I("initializing MQTT client");

    m_lastMQTTConnection = millis();
    loadOrSaveConfig_P(MQTT_CONFIG_FILE, m_mqttConfig);

    if (m_pubSubClient.connected())
    {
        LOG_I("MQTT client is already connected, reconnecting");
        m_pubSubClient.disconnect();
    }

    LOG_I("setting server %s:%d", m_mqttConfig.host.c_str(), m_mqttConfig.port);
    m_pubSubClient.setServer(m_mqttConfig.host.c_str(), m_mqttConfig.port);

    m_pubSubClient.setCallback([=](const char *topic, u8 *payload, uint length) {
        if (m_mqttConfig.initTopic == topic)
        {
            handleInitMessage(payload, length);
        }
        else if (m_mqttConfig.v1Topic)
        {
            handleV1Message(m_mqttConfig.v1Topic.c_str(), payload, length);
        }
        else
        {
            handleMessage(topic, payload, length);
        }
    });

    bool ret = false;

    LOG_I("connecting with clientID=%s, username=%s", m_mqttConfig.clientID.c_str(), m_mqttConfig.username.c_str());
    ret = m_pubSubClient.connect(m_mqttConfig.clientID.c_str(), m_mqttConfig.username.c_str(), m_mqttConfig.password.c_str());
    LOG_I("connect ret=%d", ret);

    LOG_I("subscribing to init topic %s", m_mqttConfig.initTopic.c_str());
    ret = m_pubSubClient.subscribe(m_mqttConfig.initTopic.c_str(), 0);
    LOG_I("init subscription ret=%d", ret);
    
    LOG_I("subscribing to v1 topic %s", m_mqttConfig.v1Topic.c_str());
    ret = m_pubSubClient.subscribe(m_mqttConfig.v1Topic.c_str(), 0);
    LOG_I("v1 subscription ret=%d", ret);
}

void App::handleInitMessage(const u8 *payload, uint length)
{
    bool ret = protocolDecode(payload, length, m_topics);
    if (!ret)
    {
        LOG_E("invalid init message");
    }
}

void App::handleMessage(const char *topic, const u8 *payload, uint length)
{
}

void App::handleV1Message(const char *topic, const u8 *payload, uint length)
{
    TapControl tapControl;
    bool ret = protocolDecode(payload, length, tapControl);
    LOG_I("decode message %d", ret);

    bool actionRet = false;

    if (ret)
    {
        LOG_I("cmd=%s,hash=%s", tapControl.cmd.c_str(), tapControl.hash.c_str());

        bool tapState = tapControl.cmd == "on";
        
        auto tapActuator = m_tapActuators.find(V1_PIN);
        if (tapActuator != m_tapActuators.end()) {
            tapActuator->second.setTapState(tapState);
            actionRet = true;
        };

        TapControlResponse res{actionRet ? "ok" : "fail", tapControl.hash};
        publishMessage(topic, res);

        ReportDataPoint3<DPPowerState> dp{ DPPowerState{ tapState } };
        publishMessage("$dp", dp);
    }
}
