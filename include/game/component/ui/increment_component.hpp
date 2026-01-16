#pragma once
#include <cstdint>
#include "core/data/vector2.hpp"


namespace Component::UI
{
/**
 * @brief Stores data for incremental UI elements.
 * 
 * Requires a linked float `value` pointer that will be modified by the UI element.
 * Other member variables define ways to modify the value, such as `increment` amount.
 * 
 * Usage example:
 *
 * @code
 * constexpr float INCREMENT = 0.1f;
 * constexpr Nc::Vector2f RANGE = Nc::Vector2f(0.0f, 1.0f);
 * 
 * float volume = 0.5f;
 * registry.emplace<Component::UI::Increment>(&volume, INCREMENT, RANGE);
 * @endcode
 */
struct Increment final
{
    // ------ Members ------

    float* value = nullptr;
    Nc::Vector2f range = Nc::Vector2f(0.0f, 1.0f);
    float increment = 0.1f;
    uint8_t decimals = 1u;


    // ------ Constructors ------

    constexpr Increment() = default;


    constexpr Increment(
        float* value,
        const float increment,
        const Nc::Vector2f range = Nc::Vector2f(0.0f, 1.0f),
        const uint8_t decimals = 1u
    ) noexcept
        : value(value),
          range(range),
          increment(increment),
          decimals(decimals)
    { };
};

}
