#pragma once
#include <string>

#include "entt/entity/registry.hpp"


namespace Nc {
class ResourceStore;
}


namespace Component {
struct Transform;
}


namespace Entity
{

class RuntimeAudio final
{
public:
    static entt::entity Create(
        Nc::ResourceStore& store,
        entt::registry& registry,
        const std::string& sound,
        const Component::Transform& transform,
        float volume = 1.0f,
        bool isSpacial = true,
        bool ignoreScene = false
    ) noexcept;

};

}
