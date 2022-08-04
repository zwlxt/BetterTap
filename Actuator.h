#pragma once
#include <Arduino.h>
#include "AppConfig.h"

class TapActuator
{
public:
    TapActuator(PinOutConfig pinout) : m_pinout{LED_BUILTIN, 1}
    {
        pinMode(pinout.pin, OUTPUT);
    }

    void setTapState(bool open);
private:
    PinOutConfig m_pinout;
};
