#include "watchdog.h"

void Watchdog::kick(uint32_t currentMs)
{
    m_lastKickMs = currentMs;
}

bool Watchdog::expired(
    uint32_t currentMs) const
{
    return
        (currentMs - m_lastKickMs)
        > 100;
}