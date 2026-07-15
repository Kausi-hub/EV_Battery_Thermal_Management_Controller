#pragma once

#include <fstream>

#include "telemetry_record.h"

class TelemetryLogger
{
public:

    explicit TelemetryLogger(
        const std::string& filename);

    ~TelemetryLogger();

    void log(
        const TelemetryRecord& record);

private:

    std::ofstream m_file;
};