#pragma once
#include "entt/entity/fwd.hpp"
#include "components/ui/increment_component.hpp"
#include "utility/vector2.hpp"
#include <string>
class ResourceStore;


namespace Construct
{
    void IncrementSettingObject(
        Nc::Vector2f position,
        std::string&& display,
        Component::UiIncrement&& increment,
        entt::registry& registry, 
        ResourceStore& resourceStore
    );
}