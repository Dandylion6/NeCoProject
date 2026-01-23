#include "game/system/shared/mechanical/lever_system.hpp"

#include <cmath>

#include "algorithm"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/lever_component.hpp"
#include "game/contexts/system_context.hpp"


void System::Logic::Lever::Update(const SystemContext& context) noexcept
{
    const auto view = context.registry.view<const Component::Action::Drag, Component::Logic::Lever,
                                            Component::Action::Toggle, Component::Transform>();

    for (auto [entity, drag, lever, toggle, transform] : view.each())
    {
        float targetHeight = GetHeightTarget(toggle, lever);
        if (drag.isTarget)
        {
            targetHeight = drag.startPosition.y + drag.draggedDelta.y;
            targetHeight = std::clamp<float>(targetHeight, lever.heightRange.x, lever.heightRange.y);
        }

        constexpr float STIFFNESS = 180.0f;
        constexpr float DAMPING = 19.0f;

        const float displacement = targetHeight - lever.currentHeight;
        lever.velocity += displacement * STIFFNESS * context.deltaTime;
        lever.velocity -= lever.velocity * DAMPING * context.deltaTime;
        lever.currentHeight += lever.velocity * context.deltaTime;

        transform.position.y = lever.currentHeight;
        toggle.state = GetToggleState(toggle, lever, drag.isTarget);
    }
}


float System::Logic::Lever::GetHeightTarget(
    const Component::Action::Toggle& toggle,
    const Component::Logic::Lever& lever
) noexcept
{
    switch (toggle.state)
    {
    case On: return lever.heightRange.x;
    default: return lever.heightRange.y;
    }
}


ToggleState System::Logic::Lever::GetToggleState(
    const Component::Action::Toggle& toggle,
    const Component::Logic::Lever& lever,
    const bool isHeld
) noexcept
{
    if (isHeld)
    {
        constexpr float ERROR_MARGIN = 3.0f;

        if (lever.currentHeight - ERROR_MARGIN <= lever.heightRange.x) return On;
        if (lever.currentHeight + ERROR_MARGIN >= lever.heightRange.y) return Off;
        return toggle.state;
    }

    const float distanceToOn = std::fabs(lever.currentHeight - lever.heightRange.x);
    const float distanceToOff = std::fabs(lever.currentHeight - lever.heightRange.y);
    return distanceToOn <= distanceToOff ? On : Off;
}
