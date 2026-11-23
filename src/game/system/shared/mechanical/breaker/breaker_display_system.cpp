#include "game/system/shared/mechanical/breaker/breaker_display_system.hpp"

#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/lighting/point_light_component.hpp"
#include "game/component/shared/mechanical/breaker_component.hpp"
#include "game/contexts/system_context.hpp"


void System::Logic::Breaker::Display::Update(const SystemContext& context) noexcept
{
    const auto view = context.registry.view<Component::Logic::Breaker>();
    for (const auto& [entity, breaker] : view.each())
    {
        auto& sprite = context.registry.get<Component::Sprite>(breaker.indicator);
        auto& light = context.registry.get<Component::Light::Point>(breaker.indicator);
        Nc::RGBa color = sprite.tint;
        float lightIntensity = 0.0f;

        switch (breaker.status)
        {
        case Component::Logic::Breaker::Operational:
            lightIntensity = 1.0f;
            color = Nc::RGBa(GREEN);
            break;
        case Component::Logic::Breaker::Offline:
            lightIntensity = 0.0f;
            color = Nc::RGBa(GRAY);
            break;
        case Component::Logic::Breaker::Faulted:
            lightIntensity = 1.0f;
            color = Nc::RGBa(RED);
            break;
        case Component::Logic::Breaker::Discharging:
            lightIntensity = 1.0f;
            color = Nc::RGBa(WHITE);
            break;
        case Component::Logic::Breaker::ReadyToRestart:
            lightIntensity = 1.0f;
            color = Nc::RGBa(ORANGE);
            break;
        case Component::Logic::Breaker::DesyncedRestart:
            lightIntensity = 1.0f;
            color = Nc::RGBa(PINK);
            break;
        default: break;
        }

        sprite.tint = color;
        light.strength = lightIntensity;
        light.color = color;
    }
}
