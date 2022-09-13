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
    LOG_I("initializing wifiClientSecure");

    syncTime();

    LOG_I("done");
}

void App::loop()
{
    if (m_state[StateID::syncClockComplete])
    {
        m_pubSubClient.loop();
    }
}

void App::setupActuators()
{
    LOG_I("initializing PINs");

    std::vector<PinOutConfig> pinOutConfig;
    loadOrSaveConfig_P(PIN_OUT_CONFIG, pinOutConfig);

    LOG_I("loaded %d pin configurations", pinOutConfig.size());

    for (auto cfg : pinOutConfig)
    {
        m_tapActuators.emplace(cfg.id, TapActuator{cfg});
    }

    LOG_I("completed");
}

void App::syncTime()
{
    LOG_I("starting");

    TimeConfig config;
    loadOrSaveConfig_P(TIME_CONFIG_FILE, config);

    if (config.ntpServer1.isEmpty())
    {
        LOG_E("ntpServer1 is empty");
    }

    const char *ntpServer1 = config.ntpServer1.c_str();
    const char *ntpServer2 = config.ntpServer2.isEmpty() ? nullptr : config.ntpServer2.c_str();
    const char *ntpServer3 = config.ntpServer3.isEmpty() ? nullptr : config.ntpServer3.c_str();

    settimeofday_cb([&](bool fromSNTP) {
        if (fromSNTP)
        {
            m_state.set(StateID::syncClockComplete);
            LOG_I("sync time success");

            connectMQTT();
        }
    });

    configTime(config.tz, config.dst, ntpServer1, ntpServer2, ntpServer3);
}

void App::connectMQTT()
{
    LOG_I("initializing MQTT client");

    MQTTConfig config;
    loadOrSaveConfig_P(MQTT_CONFIG_FILE, config);

    if (m_pubSubClient.connected())
    {
        m_pubSubClient.disconnect();
    }

    m_pubSubClient.setClient(m_wifiClient);
    m_pubSubClient.setServer(config.host.c_str(), config.port);
    m_pubSubClient.setCallback([=](const char *topic, u8 *payload, uint length) {
        if (config.initTopic == topic)
        {
            handleInitMessage(payload, length);
        }
        else if (config.v1Topic)
        {
            handleV1Message(config.v1Topic.c_str(), payload, length);
        }
        else
        {
            handleMessage(topic, payload, length);
        }
    });

    bool ret = m_pubSubClient.subscribe(config.initTopic.c_str(), 1);
    LOG_I("init subscription ret=%d", ret);

    m_pubSubClient.connect(config.clientID.c_str(), config.username.c_str(), config.password.c_str());
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

    bool actionRet = false;

    if (ret)
    {
        constexpr u8 TAP_ID = 1;
        if (m_tapActuators.count(TAP_ID) > 0)
        {
            auto tapActuator = m_tapActuators.find(TAP_ID);
            if (tapActuator != std::end(m_tapActuators))
            {
                tapActuator->second.setTapState(tapControl.cmd == "on");
            }
            actionRet = true;
        }
    }

    TapControlResponse res{actionRet ? "ok" : "fail", tapControl.hash};
    responseMessage(topic, res);
}
