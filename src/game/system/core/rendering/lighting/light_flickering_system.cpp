#include "game/system/core/rendering/lighting/light_flickering_system.hpp"

#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/lighting/light_source_component.hpp"
#include "game/contexts/system_context.hpp"


namespace Nc {
class Random;
}


void System::Render::LightFlickering::Update(const SystemContext& context)
{
    auto& randomService = context.registry.ctx().get<Nc::Random>();
    const auto view = context.registry.view<Component::Light::Point>();
    for (auto [entity, source] : view.each())
    {
        constexpr auto FLICKER_DELAY_RANGE = Nc::Vector2f(0.1f, 0.18f);
        constexpr float SMOOTH_SPEED = 2.9;
        constexpr float AMBIENT_RANGE = 0.12f;

        if (!source.flickers) continue;

        source.currentStrength = Nc::Math::SmoothApproach(
            source.currentStrength,
            source.targetStrength,
            context.deltaTime,
            SMOOTH_SPEED
        );

        source.flickerSecondsLeft -= context.deltaTime;
        if (source.flickerSecondsLeft > 0.0f) continue;

        source.flickerSecondsLeft = randomService.RangeFloat(FLICKER_DELAY_RANGE.x, FLICKER_DELAY_RANGE.y);
        const float rangeVariance = randomService.RangeFloat(-AMBIENT_RANGE, AMBIENT_RANGE);

        const float strength = source.strength + rangeVariance * source.strength;
        source.targetStrength = strength;
    }
}
