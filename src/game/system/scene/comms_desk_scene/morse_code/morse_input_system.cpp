#include "game/system/scene/comms_desk_scene/morse_code/morse_input_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/input_component.hpp"
#include "game/component/scene/comms_desk_scene/morse_components.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Morse::Input::Update(const SystemContext& context) noexcept
{
    bool canUseButton = false;
    switch (context.game.currentScene)
    {
    case CommsDesk: canUseButton = false; // TODO: Add transceiver + button.
        break;
    case CommsRoom: canUseButton = false;
        break;
    default: return;
    }

    const entt::entity entity = entt::get_single<Component::Morse::Transceiver>(context.registry);
    const auto& input = context.registry.get<Component::Action::Input>(entity);
    auto& transceiver = context.registry.get<Component::Morse::Transceiver>(entity);

    const bool wasPushed = transceiver.isPushed;

    // Since multiple inputs can be used for morse, it must be weighed.
    int pushWeight = 0;

    if (input.state == Component::Action::Input::Held)
        ++pushWeight;

    if (canUseButton)
    {
        const auto& click = context.registry.get<Component::Action::Click>(entity);
        if (click.isHeld) ++pushWeight;
    }

    const bool isPushed = pushWeight > 0;
    transceiver.inputJustChanged = isPushed != wasPushed;
    transceiver.isPushed = isPushed;
}
