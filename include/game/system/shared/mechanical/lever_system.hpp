#pragma once
#include "game/component/core/interactive/toggle_component.hpp"


struct SystemContext;


namespace Component::Logic
{
struct Lever;
}


namespace System::Logic
{
class Lever final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context) noexcept;
    static float GetHeightTarget(
        const Component::Action::Toggle& toggle,
        const Component::Logic::Lever& lever
    ) noexcept;

private:
    // ------ Functions ------
    static ToggleState GetToggleState(
        const Component::Action::Toggle& toggle,
        const Component::Logic::Lever& lever,
        bool isHeld
    ) noexcept;
};
}

