#pragma once
#include "core/data/vector2.hpp"
#include <string>
#include <utility>


namespace Component::Logic
{
/**
 * @brief Represents a lever entity.
 * 
 * Used with `Component::Action::Drag` to create lever mechanics.
 * Levers affect the entity's `Component::Action::Toggle` state
 * based on their position withing `heightRange`.
 * 
 * Usage example:
 * 
 * ```cpp
 * constexpr Nc::Vector2f LEVER_RANGE = Nc::Vector2f(-20.0f, 20.0f);
 * ...
 * registry.emplace<Component::Action::Drag>(entity, ...);
 * registry.emplace<Component::Action::Toggle>(entity, ...);
 * registry.emplace<Component::Logic::Lever>(
 *  entity, LEVER_RANGE, "lever_move.wav", "lever_switch.wav"
 * );
 * ```
 */
struct Lever final
{
	// ------ Members ------

    std::string movingSound { };
    std::string switchedSound { };
    // @brief Minimum and maximum lever height in world. 
    Nc::Vector2f heightRange = Nc::Vector2f::Zero();
    float currentHeight = 0.0f; 
    float velocity = 0.0f;


	// ------ Constructors ------

    constexpr Lever() noexcept = default;
    constexpr Lever(
        Nc::Vector2f heightRange, 
        std::string&& movingSound, 
        std::string&& switchedSound
    ) noexcept :
        heightRange(heightRange), 
        movingSound(std::move(movingSound)), 
        switchedSound(std::move(switchedSound)) 
    { };
};

}