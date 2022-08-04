#include "Actuator.h"

void TapActuator::setTapState(bool open)
{
    digitalWrite(m_pinout.pin, open ? m_pinout.en : 1 - m_pinout.en);
}