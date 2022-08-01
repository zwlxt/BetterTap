#pragma once

template <u8 SLOT_SIZE>
class State
{
public:
    State()
    {
        for (u8 i = 0; i < SLOT_SIZE; ++i)
        {
            slots[i] = false;
        }
    }

    void set(u8 slot)
    {
        slots[slot] = true;
    }

    void unset(u8 slot)
    {
        slots[slot] = false;
    }

    bool operator[](u8 slot) const
    {
        return slots[slot];
    }

private:
    bool slots[SLOT_SIZE];
};