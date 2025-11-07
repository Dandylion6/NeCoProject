#pragma once
#include "components/objects/interactions/lever_component.hpp"
#include "components/objects/interactions/toggle_component.hpp"
#include "entt/entity/fwd.hpp"


struct LeverSystem
{
public:
    static void Update(entt::registry& registry, float deltaTime) noexcept;

    static float GetHeightTarget(
        const Component::Toggle& toggle, const Component::Lever& lever
    ) noexcept;

private:
    static ToggleState GetToggleState(
        const Component::Toggle& toggle, 
        const Component::Lever& lever, 
        bool isHeld
    ) noexcept;

};
