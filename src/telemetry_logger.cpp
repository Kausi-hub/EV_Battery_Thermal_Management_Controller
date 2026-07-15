#include "telemetry_logger.h"
#include <cstdint>

TelemetryLogger::TelemetryLogger(
    const std::string& filename)
{
    m_file.open(filename);
    if(!m_file.is_open())
    {
        throw std::runtime_error("failed to open telemtry file");
    }
    m_file
        << "timestamp,"
        << "zone1,"
        << "zone2,"
        << "zone3,"
        << "zone4,"
        << "coolant,"
        << "current,"
        << "pumpCommand,"
        << "fanCommand,"
        << "pumpRpm,"
        << "fanRpm,"
        << "fault"
        << "\n";
}

TelemetryLogger::~TelemetryLogger()
{
    m_file.close();
}

void TelemetryLogger::log(
    const TelemetryRecord& record)
{
    m_file

        << record.timestampMs << ","

        << record.zoneTemps[0] << ","
        << record.zoneTemps[1] << ","
        << record.zoneTemps[2] << ","
        << record.zoneTemps[3] << ","

        << record.coolantTemp << ","

        << record.batteryCurrent << ","

        << record.pumpCommand << ","
        << record.fanCommand << ","

        << record.pumpRpm << ","
        << record.fanRpm << ","

        << static_cast<int>(
              record.fault)

        << "\n";
}