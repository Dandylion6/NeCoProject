#pragma once
#include "core/data/color.hpp"
#include "raylib.h"


namespace Component::Light
{
/**
 * @brief Represents a point light source in world space.
 * 
 * This component defines a simple radial light that emits from a central point.
 * The light has a color, range (radius in pixels), and adjustable strength.
 * It can be attached to an entity to simulate local lighting effects.
 * 
 * The `currentStrength` field can be dynamically adjusted during runtime
 * (e.g. flickering, fading, or pulsing effects) without modifying the base `strength`.
 * 
 * Usage example:
 * 
 * ```cpp
 * constexpr Nc::Hex COLOR = 0xffffffff;
 * constexpr float RANGE = 64.0f;
 * constexpr float STRENGTH = 1.0f;
 * 
 * registry.emplace<Component::Light::Point>(
 *  entity, COLOR, RANGE, STRENGTH
 * );
 * ```
 */
struct Point final
{
    // ------ Members ------

    Nc::RGBa color = WHITE;
    float range = 32.0f;
    float strength = 1.0f;
    float currentStrength = strength;


    // ------ Constructors ------

    constexpr Point() noexcept = default;
    constexpr Point(
        Nc::RGBa color, 
        float range, 
        float strength
    ) noexcept : 
        color(color), 
        range(range), 
        strength(strength), 
        currentStrength(strength) 
    { };
};

}