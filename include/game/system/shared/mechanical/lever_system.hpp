#pragma once
#include "entt/entity/fwd.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/lever_component.hpp"


struct LeverSystem
{
public:
    static void Update(entt::registry& registry, float deltaTime) noexcept;

    static float GetHeightTarget(
        const Component::Action::Toggle& toggle, const Component::Logic::Lever& lever
    ) noexcept;

private:
    static ToggleState GetToggleState(
        const Component::Action::Toggle& toggle, 
        const Component::Logic::Lever& lever, 
        bool isHeld
    ) noexcept;

};
