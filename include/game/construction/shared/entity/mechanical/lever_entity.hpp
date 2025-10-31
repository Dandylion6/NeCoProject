#pragma once
#include "game/component/shared/mechanical/lever_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"


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