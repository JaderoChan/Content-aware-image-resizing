#include "parameter_pack.hpp"

ParameterPack& ParameterPack::instance()
{
    static ParameterPack instance;
    return instance;
}

void ParameterPack::setCcsMethod(ComputeColorSimilarityMethod ccsMethod)
{
    ccsMethod_ = ccsMethod;
}

void ParameterPack::setIsAllDirectForEnergy(bool isAllDirectForEnergy)
{
    isAllDirectForEnergy_ = isAllDirectForEnergy;
}

ComputeColorSimilarityMethod ParameterPack::ccsMethod() const
{
    return ccsMethod_;
}

bool ParameterPack::isAllDirectForEnergy() const
{
    return isAllDirectForEnergy_;
}

void setGlobalCcsMethod(ComputeColorSimilarityMethod ccsMethod)
{
    ParameterPack::instance().setCcsMethod(ccsMethod);
}

void setGlobalIsAllDirectForEnergy(bool isAllDirectForEnergy)
{
    ParameterPack::instance().setIsAllDirectForEnergy(isAllDirectForEnergy);
}

ComputeColorSimilarityMethod globalCcsMethod()
{
    return ParameterPack::instance().ccsMethod();
}

bool globalIsAllDirectForEnergy()
{
    return ParameterPack::instance().isAllDirectForEnergy();
}
