#pragma once

#include <string>

#include "Calibration.h"

class ConfigLoader
{
public:

    static ThermalCalibration load(
        const std::string& path);
};