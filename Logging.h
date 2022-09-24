#pragma once
#include <ctime>

#define LOG_STR(x) #x
#define LOG_PATTERN(f, l) " %s " LOG_STR(f) ":" LOG_STR(l) " %s: "
#define LOG_X(level, tag, fmt, ...) Serial.printf_P(PSTR(level LOG_PATTERN(__FILE__, __LINE__) fmt "\r\n"), logger_impl::logTimestamp(), tag, ##__VA_ARGS__)
#define LOG_D(fmt, ...) LOG_X("D", __PRETTY_FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...) LOG_X("I", __PRETTY_FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) LOG_X("E", __PRETTY_FUNCTION__, fmt, ##__VA_ARGS__)

namespace logger_impl
{
    inline const char *logTimestamp()
    {
        constexpr u8 SZ = 18;
        static char timeBuffer[SZ];
        time_t now = time(nullptr);
        strftime(timeBuffer, SZ, "%y-%m-%d %T", localtime(&now));
        return timeBuffer;
    }
}