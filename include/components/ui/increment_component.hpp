#pragma once
#include <cstdint>


namespace Component 
{
    struct UiIncrement
    {
        float* value = nullptr;
        float increment = 0.1f;
        uint8_t decimals = 1u;

        UiIncrement() = default;
        UiIncrement(float* value, float increment, uint8_t decimals = 1u): 
            value(value), increment(increment), decimals(decimals) { };
    };
}