#pragma once

#include <atomic>

#include "aux_tools.hpp"

class ParameterPack
{
public:
    static ParameterPack& instance();

    void setCcsMethod(ComputeColorSimilarityMethod ccsMethod);
    void setIsAllDirectForEnergy(bool isAllDirectForEnergy);

    ComputeColorSimilarityMethod ccsMethod() const;
    bool isAllDirectForEnergy() const;

private:
    ParameterPack() = default;
    ~ParameterPack() = default;
    ParameterPack(const ParameterPack& other) = delete;
    ParameterPack& operator=(const ParameterPack& other) = delete;

    std::atomic<ComputeColorSimilarityMethod> ccsMethod_ = CCS_NORMAL;
    std::atomic<bool> isAllDirectForEnergy_ = false;
};

void setGlobalCcsMethod(ComputeColorSimilarityMethod ccsMethod);

void setGlobalIsAllDirectForEnergy(bool isAllDirectForEnergy);

ComputeColorSimilarityMethod globalCcsMethod();

bool globalIsAllDirectForEnergy();
