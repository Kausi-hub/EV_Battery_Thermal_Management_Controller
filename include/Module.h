#pragma once

#include <vector>
#include "Cell.h"

class Module
{
public:
    explicit Module(std::size_t numCells);

    void update(
        float current,
        float coolantTemp,
        float coolingFactor,
        float dt);

    float getAverageTemperature() const;

    float getMaxCellTemp() const;
    float getMinCellTemp() const;

    float getCellImbalance() const;

    const std::vector<Cell>& getCells() const;
    std::vector<float> getCellTemperatures() const;
    void applyCellTemperatures(const std::vector<float>& temps);

private:
    std::vector<Cell> m_cells;
};