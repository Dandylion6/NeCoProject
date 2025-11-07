#pragma once
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/shared/mechanical/lever_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "entt/entity/fwd.hpp"


struct LeverSystem
{
public:
    static void Update(entt::registry& registry, float deltaTime) noexcept;

private:
    static ToggleState GetToggleState(
        const Component::Action::Toggle& toggle, 
        const Component::Lever& lever, 
        bool isHeld
    ) noexcept;

    static float GetHeightTarget(
        const Component::Action::Toggle& toggle, const Component::Lever& lever
    ) noexcept;
};
