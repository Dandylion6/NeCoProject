#include "game/system/scene/comms_scene/radar/blip/glitch/blip_signal_noise_system.hpp"

#include <format>

#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/contexts/system_context.hpp"


void System::Blip::TextError::Initialize(entt::registry& registry, const entt::entity entity, Component::Blip& blip)
{
    blip.state = Component::Blip::CoordinateError;
    registry.emplace<Component::Glitch::SignalNoise>(entity);
}


void System::Blip::TextError::Update(const SystemContext& context)
{
    const auto view = context.registry.view<Component::Blip, Component::Text, Component::Glitch::SignalNoise>();
    for (auto [entity, blip, text, error] : view.each())
    {
        if (blip.remainingGlitchSeconds <= 0.0f)
        {
            blip.state = Component::Blip::Stable;
            context.registry.remove<Component::Glitch::SignalNoise>(entity);
            return;
        }

        UpdateCharacters(context, error);

        // Construct the display string.
        text.text = std::format("({}{} , {}{})", error.glitchedCharacters[0], error.glitchedCharacters[1], error.glitchedCharacters[2], error.glitchedCharacters[3]);
    }
}


void System::Blip::TextError::UpdateCharacters(const SystemContext& context, Component::Glitch::SignalNoise& error)
{
    auto& randomService = context.registry.ctx().get<Nc::Random>();

    for (uint8_t i = 0u; i < 4u; ++i)
    {
        constexpr Nc::Vector2f RANGE = Component::Glitch::SignalNoise::GLITCH_INTERVAL_RANGE;

        error.glitchSecondsLeft[i] -= context.deltaTime;;
        if (error.glitchSecondsLeft[i] > 0.0f) continue;

        const char randomCharacter = static_cast<char>(randomService.RangeInt(32, 126));
        error.glitchedCharacters[i] = randomCharacter;

        const float randomValue = randomService.RangeFloat(0.0f, 1.0f);
        error.glitchSecondsLeft[i] = Nc::Math::Lerp(RANGE.x, RANGE.y, randomValue);
    }
}
