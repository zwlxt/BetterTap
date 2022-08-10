#pragma once

class State
{
    using Inner = u8;

public:
    State() : m_slots(0) {}

    void set(u8 slot)
    {
        m_slots |= 1 << slot;
    }

    void unset(u8 slot)
    {
        m_slots &= 0 << slot;
    }

    bool operator[](u8 slot) const
    {
        return (m_slots >> slot) & 0x01;
    }

    size_t size() const
    {
        return sizeof(Inner);
    }

private:
    Inner m_slots;
};
