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
        constexpr float SMOOTH_SPEED = 2.4f;

        if (!source.flickers) continue;

        source.flickerSecondsLeft -= context.deltaTime;
        if (source.flickerSecondsLeft <= 0.0f)
        {
            source.flickerSecondsLeft = randomService.RangeFloat(0.06f, 0.18f);
            const float rangeVariance = randomService.RangeFloat(-0.8f, 0.1f);

            const float strength = source.strength + rangeVariance * source.strength;
            source.targetStrength = strength;
        }

        source.currentStrength = Nc::Math::SmoothApproach(
            source.currentStrength,
            source.targetStrength,
            context.deltaTime,
            SMOOTH_SPEED
        );
    }
}
