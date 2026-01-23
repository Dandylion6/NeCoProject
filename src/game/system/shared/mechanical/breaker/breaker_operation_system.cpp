#include "game/system/shared/mechanical/breaker/breaker_operation_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/breaker_component.hpp"
#include "game/contexts/system_context.hpp"


void System::Logic::Breaker::Operation::Update(const SystemContext& context) noexcept
{
    const auto view = context.registry.view<const Component::Action::Toggle, Component::Logic::Breaker>();
    for (auto [entity, toggle, breaker] : view.each())
    {
        if (!context.registry.all_of<Component::Action::Toggle>(breaker.system)) continue;
        auto& systemToggle = context.registry.get<Component::Action::Toggle>(breaker.system);

        if (systemToggle.state == Disabled) continue;

        // Breaker overrides system toggle.
        const bool powerIsCut = toggle.state == Off;
        if (powerIsCut && systemToggle.state == On)
            systemToggle.state = Off;

        switch (toggle.state)
        {
        case On: breaker.status = Component::Logic::Breaker::Operational;
            break;
        case Off: breaker.status = Component::Logic::Breaker::Offline;
            break;
        default: break;
        }
    }
}
