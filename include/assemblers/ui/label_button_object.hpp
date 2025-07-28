#pragma once
#include "components/core/transform_component.hpp"
#include "entt/entity/fwd.hpp"
#include <functional>
class ResourceStore;


struct LabelButton
{
    const entt::entity label;
    const entt::entity button;

    LabelButton(const entt::entity label, const entt::entity button): label(label), button(button) { }; 
};


namespace Construct 
{
    LabelButton LabelButtonObject(
        Component::UiTransform&& transform,
        std::string&& display,
        std::function<void()>&& onClick,
        entt::registry& registry,
        ResourceStore& resourceStore
    );
}