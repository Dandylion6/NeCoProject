#pragma once
#include "entt/entity/entity.hpp"
#include "core/data/vector2.hpp"
#include <utility>
#include <cstdint>
#include <string>
#include <utility>


namespace Component
{
    struct Lever
    {
        std::string movingSound { };
        std::string switchedSound { };
        float currentHeight = 0.0f;
        float velocity = 0.0f;
        /// @brief Minimum and maximum lever height in world. 
        Nc::Vector2f heightRange = Nc::Vector2f::Zero();

        Lever() = default;
        Lever(Nc::Vector2f heightRange, std::string&& movingSound, std::string&& switchedSound) :
            heightRange(heightRange), movingSound(std::move(movingSound)), switchedSound(std::move(switchedSound)) { };
    };
}