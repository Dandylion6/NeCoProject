#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <string>
namespace Component
{
    struct Transform;
    struct Lever;
}


namespace Assembled
{
    struct LeverData
    {
        Component::Lever& lever;
        entt::entity entity = entt::null;
    };
}


namespace Construct
{
    Assembled::LeverData LeverEntity(
        entt::registry& registry, 
        const entt::entity entity,
        Nc::Vector2f heightRange,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;

    Assembled::LeverData LeverEntity(
        entt::registry& registry, 
        const entt::entity entity,
        Nc::Vector2f heightRange, 
        Texture2D&& handleTexture,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;

    Assembled::LeverData LeverEntity(
        entt::registry& registry, 
        Component::Transform&& transform,
        float moveRange,
        Texture2D&& handleTexture,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;

    Assembled::LeverData LeverEntity(
        entt::registry& registry, 
        Component::Transform&& transform,
        float moveRange,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;
}