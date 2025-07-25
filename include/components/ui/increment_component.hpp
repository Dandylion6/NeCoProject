#pragma once
#include <cstdint>
#include <functional>
#include <utility>


namespace Component 
{
    struct UiIncrement
    {
        std::function<float(float)> onIncrement { };
        float increment = 0.1f;
        uint8_t decimals = 1u;

        UiIncrement() = default;
        UiIncrement(float increment, uint8_t decimals = 1u): increment(increment), decimals(decimals) { };
    };
}