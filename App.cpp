#include <vector>
#include <coredecls.h>
#include "Constants.h"
#include "App.h"
#include "AppConfig.h"
#include "Logging.h"

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
    loadOrSaveConfig(PIN_OUT_CONFIG, pinOutConfig);

    LOG_I("loaded %d pin configurations", pinOutConfig.size());

    for (auto cfg : pinOutConfig)
    {
        m_tapActuators.emplace(cfg.id, TapActuator {cfg});
    }

    LOG_I("completed");
}

void App::syncTime()
{
    LOG_I("starting");

    TimeConfig config;
    loadOrSaveConfig(TIME_CONFIG_FILE, config);

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
    loadOrSaveConfig(MQTT_CONFIG_FILE, config);

    if (m_pubSubClient.connected())
    {
        m_pubSubClient.disconnect();
    }

    TopicConfig topics;
    loadOrSaveConfig(TOPIC_CONFIG_FILE, topics);

    m_pubSubClient.setClient(m_wifiClient);
    m_pubSubClient.setServer(config.host.c_str(), config.port);
    m_pubSubClient.setCallback([=](const char *topic, u8 *payload, uint length)
                               { handleMessage(topic, payload, length, topics); });

    m_pubSubClient.connect(config.clientID.c_str(), config.username.c_str(), config.password.c_str());
}

void App::handleMessage(const char *topic, const u8 *payload, uint length, const TopicConfig &topics)
{
    if (topics.legacyAction == topic)
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
}
