#pragma once
#include <ctime>

#define LOG_STR(x) #x
#define LOG_PATTERN(f, l) " %s " LOG_STR(f) ":" LOG_STR(l) " [%s]: "
#define LOG_X(level, tag, fmt, ...) Serial.printf_P(PSTR(level LOG_PATTERN(__FILE__, __LINE__) fmt "\n"), logger_impl::logTimestamp(), tag, ##__VA_ARGS__)
#define LOG_D(fmt, ...) LOG_X("D", logger_impl::logMethodName(__PRETTY_FUNCTION__), fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...) LOG_X("I", logger_impl::logMethodName(__PRETTY_FUNCTION__), fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) LOG_X("E", logger_impl::logMethodName(__PRETTY_FUNCTION__), fmt, ##__VA_ARGS__)

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

    inline const char *logMethodName(const char *prettyFunction)
    {
        char *cur = (char *)prettyFunction;
        char c;
        while ((c = *cur++) != '\0')
        {
            if (c == ' ')
            {
                return cur;
            }
        }

        return prettyFunction;
    }
}