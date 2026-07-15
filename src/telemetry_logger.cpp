#include "telemetry_logger.h"

#include <fstream>
#include <iostream>
#include <filesystem>

TelemetryLogger::TelemetryLogger(
    const std::string& filename)
{
    m_file.open(filename);

    if (!m_file.is_open())
    {
        std::cerr
            << "Failed to open "
            << filename
            << std::endl;
        return;
    }

    m_file
        << "TimeMs,"
        << "Temp1,"
        << "Temp2,"
        << "Temp3,"
        << "Temp4,"
        << "CoolantTemp,"
        << "Current,"
        << "PumpCmd,"
        << "FanCmd,"
        << "PumpRpm,"
        << "FanRpm,"
        << "Fault"
        << '\n';
}

TelemetryLogger::~TelemetryLogger()
{
    if (m_file.is_open())
    {
        m_file.flush();
        m_file.close();
    }
}

void TelemetryLogger::log(
    const TelemetryRecord& r)
{
    if (!m_file.is_open()){
        std::cerr << "File not open during log()" << std::endl;
        return;
    }
    if(!m_file.good()){
        std::cerr << "File stream error" << std::endl;
        return;
    }

    m_file
        << r.timestampMs << ","
        << r.zoneTemps[0] << ","
        << r.zoneTemps[1] << ","
        << r.zoneTemps[2] << ","
        << r.zoneTemps[3] << ","
        << r.coolantTemp << ","
        << r.batteryCurrent << ","
        << r.pumpCommand << ","
        << r.fanCommand << ","
        << r.pumpRpm << ","
        << r.fanRpm << ","
        << static_cast<int>(r.fault)
        << '\n';
    if (!m_file.good()){std::cerr << "write failed" << std::endl;}
    m_file.flush();
}