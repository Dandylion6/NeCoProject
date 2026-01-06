#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include <string>
namespace Component
{
    struct Transform;
    namespace Logic { struct Lever; };
}


namespace Entity
{

class Lever
{
public:
    struct Data
    {
        Component::Logic::Lever& lever;
        entt::entity entity = entt::null;
    };


    static Data Create(
        entt::registry& registry,
        entt::entity entity,
        Nc::Vector2f heightRange,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;


    static Data Create(
        entt::registry& registry,
        entt::entity entity,
        Nc::Vector2f heightRange,
        Texture2D&& handleTexture,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;


    static Data Create(
        entt::registry& registry,
        Component::Transform&& transform,
        float moveRange,
        Texture2D&& handleTexture,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;


    static Data Create(
        entt::registry& registry,
        Component::Transform&& transform,
        float moveRange,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;

};

}