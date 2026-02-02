#include "game/system/shared/mechanical/breaker/breaker_display_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/shared/mechanical/breaker_component.hpp"
#include "game/contexts/system_context.hpp"


void System::Logic::Breaker::Display::Update(const SystemContext& context) noexcept
{
    const auto view = context.registry.view<Component::Logic::Breaker>();
    for (const auto& [entity, breaker] : view.each())
    {
        // TODO: Replace placeholder visuals.
        auto& rectangle = context.registry.get<Component::Rectangle>(breaker.indicator);

        switch (breaker.status)
        {
        case Component::Logic::Breaker::Operational: rectangle.fillColor = Nc::RGBa(GREEN);
            break;
        case Component::Logic::Breaker::Offline: rectangle.fillColor = Nc::RGBa(WHITE);
            break;
        case Component::Logic::Breaker::Faulted: rectangle.fillColor = Nc::RGBa(RED);
            break;
        case Component::Logic::Breaker::Discharging: rectangle.fillColor = Nc::RGBa(WHITE);
            break;
        case Component::Logic::Breaker::ReadyToRestart: rectangle.fillColor = Nc::RGBa(ORANGE);
            break;
        case Component::Logic::Breaker::DesyncedRestart: rectangle.fillColor = Nc::RGBa(PINK);
            break;
        default: break;
        }
    }
}
