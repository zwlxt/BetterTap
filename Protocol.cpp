#include "Protocol.h"

namespace protocol_adapter_impl
{
    bool protocolDecode(const u8 *bufferIn, size_t length, TapControl &out)
    {
        StaticJsonDocument<96> doc;
        DeserializationError e = deserializeJson(doc, (const char *)bufferIn, length);

        if (e)
        {
            LOG_E("err: %s", e.f_str());
            return false;
        }

        out.cmd = doc["cmd"].as<String>();
        out.hash = doc["hash"].as<String>();

        return out.cmd == "on" || out.cmd == "off";
    }

    void protocolEncode(const TapControlResponse &in, Print& out)
    {
        StaticJsonDocument<96> doc;
        doc["response"] = in.response;
        doc["hash"] = in.hash;

        serializeJson(doc, out);
    }

    bool protocolDecode(const u8 *bufferIn, size_t length, TopicConfig &out)
    {
        
    }

    void protocolEncode(const DPPowerState &in, Print& out)
    {
        StaticJsonDocument<16> doc;
        doc["power"] = in.power;

        serializeJson(doc, out);
    }    
}