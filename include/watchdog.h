#pragma once

#include <cstdint>

class Watchdog
{
public:

    void kick(uint32_t currentMs);

    bool expired(
        uint32_t currentMs) const;

private:

    uint32_t m_lastKickMs = 0;
};