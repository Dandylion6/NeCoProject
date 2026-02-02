#include "game/system/ui/buttons/increment_buttons_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/ui/increment_button_component.hpp"
#include "game/component/ui/increment_component.hpp"
#include "game/contexts/system_context.hpp"


void System::UI::IncrementButtons::Update(const SystemContext& context)
{
    const auto view = context.registry.view<Component::Action::Click, Component::UI::IncrementButton>();
    for (auto [entity, click, incrementButton] : view.each())
    {
        if (!click.justClicked) continue;

        auto& increment = context.registry.get<Component::UI::Increment>(incrementButton.increment);
        if (increment.value == nullptr) continue;

        float newValue = *increment.value;
        switch (incrementButton.type)
        {
        case Component::UI::IncrementButton::Decrease:
            newValue -= increment.increment;
            break;
        case Component::UI::IncrementButton::Increase:
            newValue += increment.increment;
            break;
        }
        *increment.value = newValue;
        increment.hasChanged = true;
    }
}
