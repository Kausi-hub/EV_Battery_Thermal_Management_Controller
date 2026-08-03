#include "Module.h"

#include <algorithm>
#include <numeric>

Module::Module(std::size_t numCells)
{
    m_cells.reserve(numCells);

    for (std::size_t i = 0; i < numCells; ++i)
    {
        m_cells.emplace_back(
            5.0f,   // capacity (Ah)
            0.002f  // internal resistance
        );
    }
}

void Module::update(float current, float coolantTemp, float coolingFactor, float dt)
{
    float cellCurrent = current / static_cast<float>(m_cells.size());

    for (auto& cell : m_cells)
    {
        cell.update(cellCurrent, coolantTemp, coolingFactor, dt);
    }
}

std::vector<float> Module::getCellTemperatures() const
{
    std::vector<float> temps;

    for(const auto& cell : m_cells)
    {
        temps.push_back(
            cell.getTemperature());
    }

    return temps;
}

float Module::getAverageTemperature() const
{
    if (m_cells.empty())
    {
        return 0.0f;
    }

    float sum = 0.0f;

    for (const auto& cell : m_cells)
    {
        sum += cell.getTemperature();
    }

    return sum / static_cast<float>(m_cells.size());
}

float Module::getMaxCellTemp() const
{
    float maxTemp = -1000.0f;

    for (const auto& cell : m_cells)
    {
        maxTemp = std::max(maxTemp, cell.getTemperature());
    }

    return maxTemp;
}

float Module::getMinCellTemp() const
{
    float minTemp = 1000.0f;

    for (const auto& cell : m_cells)
    {
        minTemp = std::min(minTemp, cell.getTemperature());
    }

    return minTemp;
}

float Module::getCellImbalance() const
{
    return getMaxCellTemp() - getMinCellTemp();
}

const std::vector<Cell>& Module::getCells() const
{
    return m_cells;
}

void Module::applyCellTemperatures(
    const std::vector<float>& temps)
{
    for(std::size_t i = 0;
        i < m_cells.size() &&
        i < temps.size();
        ++i)
    {
        m_cells[i].setTemperature(
            temps[i]);
    }
}