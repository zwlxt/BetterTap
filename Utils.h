#pragma once

#include <Arduino.h>
#include <vector>

class BytePrint : public Print
{
public:
    BytePrint() noexcept
    {
        m_buffer.reserve(8);
    }

    inline const uint8_t *data() const
    {
        return m_buffer.data();
    }

    inline size_t size() const
    {
        return m_buffer.size();
    }

    const char *to_str()
    {
        write(0);
        return (const char *)data();
    }

    virtual size_t write(uint8_t b) override
    {
        m_buffer.push_back(b);
        return 1;
    }
private:
    std::vector<uint8_t> m_buffer;
};