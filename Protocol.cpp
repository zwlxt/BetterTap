#include "Protocol.h"

namespace protocol_adapter_impl
{
    bool protocolDecode(const u8 *bufferIn, size_t length, TapControl &out)
    {
        StaticJsonDocument<64> doc;
        DeserializationError e = deserializeJson(doc, (const char *)bufferIn);

        if (e)
        {
            out.cmd = doc["cmd"].as<String>();
            out.hash = doc["hash"].as<String>();

            return true;
        }

        return false;
    }

    void protocolEncode(const TapControlResponse &in, Print& out)
    {
        StaticJsonDocument<64> doc;
        doc["response"] = in.response;
        doc["hash"] = in.hash;

        serializeJson(doc, out);
    }
}