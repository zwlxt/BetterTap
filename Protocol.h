#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

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
}

namespace protocol_adapter_impl
{
    using namespace std;
    using namespace protocol_onenet;
    using namespace protocol_v1;

    bool protocolDecode(const u8 *bufferIn, size_t length, TapControl &out);
    void protocolEncode(const TapControlResponse &in, Print& out);

    template <typename T>
    void protocolEncode(const ReportDataPoint3<T> &in, Print &out)
    {
        out.write(0x03);
        
        String dataBytes;
        protocolEncode(in.data, dataBytes);

        out.write(highByte(dataBytes.length()));
        out.write(lowByte(dataBytes.length()));

        out.write(dataBytes.c_str(), dataBytes.length());
    }
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