#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include "Utils.h"
#include "Logging.h"

namespace protocol_onenet
{
    template <typename T>
    struct ReportDataPoint3
    {
        T data;
    };
}

namespace protocol_v1
{
    struct TapControl
    {
        String cmd;
        String hash;
    };

    struct TapControlResponse
    {
        String response;
        String hash;
    };

    struct DPPowerState
    {
        u8 power;
    };
}

namespace protocol_v2
{
    struct Command
    {
        String code;
    };

    template <typename T>
    struct Response
    {
        String code;
        T data;
    };

    struct TopicConfig
    {
    };
}

namespace protocol_adapter_impl
{
    using namespace std;
    using namespace protocol_onenet;
    using namespace protocol_v1;
    using namespace protocol_v2;

    bool protocolDecode(const u8 *bufferIn, size_t length, TopicConfig &out);

    bool protocolDecode(const u8 *bufferIn, size_t length, TapControl &out);
    void protocolEncode(const TapControlResponse &in, Print& out);

    template <typename T>
    void protocolEncode(const ReportDataPoint3<T> &in, Print &out)
    {
        out.write((u8)0x03);

        BytePrint dataBytes;
        protocolEncode(in.data, dataBytes);

        u16 len = (u16) dataBytes.size() & 0xFFFF;

        out.write(highByte(len));
        out.write(lowByte(len));

        out.write(dataBytes.data(), len);
    }

    void protocolEncode(const DPPowerState &in, Print& out);
}

template <typename T>
bool protocolDecode(const u8 *bufferIn, size_t length, T &out)
{
    return protocol_adapter_impl::protocolDecode(bufferIn, length, out);
}

template <typename T>
void protocolEncode(const T &in, Print &out)
{
    protocol_adapter_impl::protocolEncode(in, out);
}