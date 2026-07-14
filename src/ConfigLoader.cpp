#include "ConfigLoader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

ThermalCalibration ConfigLoader::load(
    const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open calibration file: " +
            path);
    }

    std::unordered_map<std::string, float> values;

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        if (line[0] == '#')
        {
            continue;
        }

        std::size_t delimiter =
            line.find('=');

        if (delimiter == std::string::npos)
        {
            continue;
        }

        std::string key =
            line.substr(0, delimiter);

        std::string valueString =
            line.substr(delimiter + 1);

        try
        {
            values[key] =
                std::stof(valueString);
        }
        catch (...)
        {
            throw std::runtime_error(
                "Invalid calibration value for key: " +
                key);
        }
    }

    ThermalCalibration calibration {};

    calibration.targetTemp =
        values.at("targetTemp");

    calibration.normalThreshold =
        values.at("normalThreshold");

    calibration.criticalThreshold =
        values.at("criticalThreshold");

    calibration.safeThreshold =
        values.at("safeThreshold");

    return calibration;
}