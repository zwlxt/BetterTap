#pragma once
#include <Arduino.h>
#include <vector>

namespace protocol_onenet
{
    template <typename T>
    struct ReportDataPoint3
    {
        T data;
    };
}

namespace adapter_impl
{
    using namespace std;
    using namespace protocol_onenet;

    template <typename T>
    vector<u8> protocolEncode(const ReportDataPoint3<T> &in)
    {
        vector<u8> buffer;
        buffer.push_back(0x03);

        vector<u8> dataBytes = protocolEncode(in.data);

        buffer.push_back(highByte(dataBytes.size()));
        buffer.push_back(lowByte(dataBytes.size()));

        buffer.insert(end(buffer), begin(dataBytes), end(dataBytes));

        return buffer;
    }

    template <typename T>
    vector<u8> protocolDecode(const u8 *bufferIn, size_t length, ReportDataPoint3<T> &out)
    {
        return vector<u8>();
    }
}

template <typename T>
bool protocolDecode(const u8 *bufferIn, size_t length, T &out)
{
    return adapter_impl::protocolDecode(bufferIn, length, out);
}

template <typename T>
std::vector<u8> protocolEncode(const T &in)
{
    return adapter_impl::protocolEncode(in);
}