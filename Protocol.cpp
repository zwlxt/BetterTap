#include "Protocol.h"

namespace protocol_adapter_impl
{
    bool protocolDecode(const u8 *bufferIn, size_t length, TapControl &out)
    {
        StaticJsonDocument<64> doc;
        deserializeJson(doc, String((const char *)bufferIn));

        out.cmd = doc["cmd"].as<String>();
        out.hash = doc["hash"].as<String>();
    }
}