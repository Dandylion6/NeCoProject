#include "game/system/core/rendering/lighting/light_flickering_system.hpp"

#include "core/math/interpolation.hpp"
#include "core/math/nc_math.hpp"
#include "core/math/random.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/rendering/lighting/point_light_component.hpp"
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
        constexpr auto FLICKER_DELAY_MIN_STRENGTH_RANGE = Nc::Vector2f(0.14f, 0.69f);
        constexpr auto FLICKER_DELAY_MAX_STRENGTH_RANGE = Nc::Vector2f(0.04f, 0.08f);
        constexpr auto SMOOTH_SPEED_RANGE = Nc::Vector2f(0.2f, 3.6f);
        constexpr auto LIGHT_STRENGTH_RANGE = Nc::Vector2f(0.0f, 1.16f);

        source.currentStrength = Nc::Math::SmoothApproach(
            source.currentStrength,
            source.targetStrength,
            context.deltaTime,
            Nc::Math::Lerp(SMOOTH_SPEED_RANGE.x, SMOOTH_SPEED_RANGE.y, source.currentStrength)
        );

        if (source.flickerStrength <= Nc::Math::EPSILON)
        {
            source.targetStrength = source.strength; // Goes back to base value.
            continue;
        }

        source.flickerSecondsLeft -= context.deltaTime;
        if (source.flickerSecondsLeft > Nc::Math::EPSILON) continue;

        const Nc::Vector2f delayRange = Nc::Vector::Lerp(FLICKER_DELAY_MIN_STRENGTH_RANGE, FLICKER_DELAY_MAX_STRENGTH_RANGE, source.flickerStrength);
        source.flickerSecondsLeft = randomService.RangeFloat(delayRange.x, delayRange.y);

        const float flickerRange = Nc::Math::Lerp(LIGHT_STRENGTH_RANGE.x, LIGHT_STRENGTH_RANGE.y, source.flickerStrength);
        const float rangeVariance = randomService.RangeFloat(-flickerRange * 0.85f, flickerRange * 0.15f);

        const float strength = source.strength + rangeVariance * source.strength;
        source.targetStrength = std::fmaxf(strength, 0.0f);
    }
}
