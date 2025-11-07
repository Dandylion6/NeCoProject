#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/component/shared/mechanical/lever_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/shared/mechanical/lever_system.hpp"


void LeverSystem::Update(entt::registry &registry, float deltaTime) noexcept
{
    auto view = registry.view<const Component::Action::Drag, Component::Lever, Component::Action::Toggle, Component::Transform>();
    for (auto [entity, drag, lever, toggle, transform] : view.each())
    {
        float targetHeight = GetHeightTarget(toggle, lever);
        if (drag.isTarget)
        {
            targetHeight = drag.startPosition.y + drag.draggedDelta.y;
            targetHeight = std::clamp<float>(targetHeight, lever.heightRange.x, lever.heightRange.y);
        }
        
        constexpr float STIFFNESS = 180.0f;
        constexpr float DAMPING   = 19.0f;

        float displacement = targetHeight - lever.currentHeight;
        lever.velocity += displacement * STIFFNESS * deltaTime;
        lever.velocity -= lever.velocity * DAMPING * deltaTime;
        lever.currentHeight += lever.velocity * deltaTime;

        transform.position.y = lever.currentHeight;
        toggle.state = GetToggleState(toggle, lever, drag.isTarget);
    }
}


float LeverSystem::GetHeightTarget(const Component::Toggle& toggle, const Component::Lever& lever) noexcept
{
    switch (toggle.state)
    {
    case On: return lever.heightRange.x;
    default: return lever.heightRange.y;
    }
    return lever.heightRange.y;
}


ToggleState LeverSystem::GetToggleState(
    const Component::Action::Toggle& toggle, 
    const Component::Lever& lever, 
    bool isHeld
) noexcept
{
    constexpr float ERROR_MARGIN = 3.0f;

    if (isHeld)
    {
        if (lever.currentHeight - ERROR_MARGIN <= lever.heightRange.x) return On;
        if (lever.currentHeight + ERROR_MARGIN >= lever.heightRange.y) return Off;
        return toggle.state;
    }

    float distanceToOn = std::fabsf(lever.currentHeight - lever.heightRange.x);
    float distanceToOff = std::fabsf(lever.currentHeight - lever.heightRange.y);
    return distanceToOn <= distanceToOff ? On : Off;
}


float LeverSystem::GetHeightTarget(const Component::Action::Toggle& toggle, const Component::Lever& lever) noexcept
{
    switch (toggle.state)
    {
    case On: return lever.heightRange.x;
    default: return lever.heightRange.y;
    }
    return lever.heightRange.y;
}