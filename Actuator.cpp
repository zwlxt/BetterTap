#include "Actuator.h"

void TapActuator::setTapState(bool open)
{
    u8 value = open ? m_pinout.en : 1 - m_pinout.en;
    LOG_I("tap state=%d", value);
    digitalWrite(m_pinout.pin, value);
}