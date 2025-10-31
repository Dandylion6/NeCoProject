#pragma once
#include "raylib.h"
#include "core/data/color.hpp"


namespace Component
{
    struct LightSource
    {
        Nc::RGBa color = WHITE;
        float range = 32.0f;
        float strength = 1.0f;
        float currentStrength = strength;

        LightSource() = default;
        LightSource(Nc::RGBa color, float range, float strength): 
            color(color), range(range), strength(strength), currentStrength(strength) { };
    };
}