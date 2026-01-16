#pragma once
#include <string>

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/fwd.hpp"


namespace Component
{
struct Transform;
namespace Logic { struct Lever; };
}


namespace Entity
{

class Lever final
{
public:
    // ------ Types ------
    struct Data final
    {
        Component::Logic::Lever& lever;
        entt::entity entity = entt::null;
    };


    // ------ Functions ------

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
        const Texture2D& handleTexture,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;


    static Data Create(
        entt::registry& registry,
        const Component::Transform& transform,
        float moveRange,
        const Texture2D& handleTexture,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;


    static Data Create(
        entt::registry& registry,
        const Component::Transform& transform,
        float moveRange,
        std::string&& movingAudioFile,
        std::string&& switchedAudioFile
    ) noexcept;

};

}
