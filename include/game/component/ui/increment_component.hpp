#pragma once
#include "utility/vector2.hpp"
#include <cstdint>


namespace Component::UI
{
    struct Increment
    {
        float* value = nullptr;
        Nc::Vector2f range = Nc::Vector2f(0.0f, 1.0f);
        float increment = 0.1f;
        uint8_t decimals = 1u;

        UiIncrement() = default;
        UiIncrement(
            float* value,
            float increment,
            Nc::Vector2f range = Nc::Vector2f(0.0f, 1.0f),
            uint8_t decimals = 1u
        ): 
            value(value), 
            increment(increment), 
            range(range), 
            decimals(decimals) 
        { };
    };
}