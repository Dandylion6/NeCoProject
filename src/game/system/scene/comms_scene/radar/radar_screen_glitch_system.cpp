#include "game/system/scene/comms_scene/radar/radar_screen_glitch_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/math/random.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/contexts/system_context.hpp"


void System::Radar::ScreenGlitch::Update(const SystemContext& context)
{
    constexpr float BASE_VOLUME = 0.1;
    constexpr float GLITCH_VOLUME_MULTIPLIER = 1.42f;

    const entt::entity entity = entt::get_single<Component::Radar>(context.registry);
    auto& radar = context.registry.get<Component::Radar>(entity);
    auto& emitter = context.registry.get<Component::AudioEmitter>(radar.screen);
    auto& toggle = context.registry.get<Component::Action::Toggle>(entity);

    if (radar.isTurningOff)
    {
        radar.turningOffSecondsLeft -= context.deltaTime;
        if (radar.turningOffSecondsLeft <= 0.0f)
        {
            radar.isTurningOff = false;
            toggle.state = Off;
        }
    }

    if (toggle.state != On)
    {
        emitter.volume = 0.0f; // No noise when off.
        return;
    }

    radar.screenGlitchWaitSecondsLeft -= context.deltaTime;
    radar.screenGlitchSecondsLeft = std::max(radar.screenGlitchSecondsLeft - context.deltaTime, 0.0f);

    const float glitchAlpha = radar.screenGlitchSeconds > 0.0f ? radar.screenGlitchSecondsLeft / radar.screenGlitchSeconds : 0.0f;
    emitter.volume = BASE_VOLUME + radar.screenGlitchStrength * glitchAlpha * GLITCH_VOLUME_MULTIPLIER;

    if (radar.screenGlitchWaitSecondsLeft > 0.0f) return;

    auto& randomService = context.registry.ctx().get<Nc::Random>();
    const float uniform = randomService.RangeFloat(0.0f, 1.0f);
    const float intensity = uniform * uniform;

    StartGlitch(context, intensity);
}


void System::Radar::ScreenGlitch::StartGlitch(const SystemContext& context, float intensity)
{
    constexpr auto WAIT_RANGE = Nc::Vector2f(9.6f, 40.0f);
    constexpr auto DURATION_RANGE = Nc::Vector2f(0.12f, 0.64f);
    constexpr auto STRENGTH_RANGE = Nc::Vector2f(0.1f, 0.6f);

    auto& randomService = context.registry.ctx().get<Nc::Random>();

    const entt::entity entity = entt::get_single<Component::Radar>(context.registry);
    auto& radar = context.registry.get<Component::Radar>(entity);

    radar.screenGlitchSeconds = Nc::Math::Lerp(DURATION_RANGE.x, DURATION_RANGE.y, intensity);
    radar.screenGlitchStrength = Nc::Math::Lerp(STRENGTH_RANGE.x, STRENGTH_RANGE.y, intensity);
    radar.screenGlitchSecondsLeft = radar.screenGlitchSeconds;

    intensity += randomService.RangeFloat(-0.1f, 0.1f); // Skew the duration slightly.
    intensity = std::max(intensity, 0.0f);
    radar.screenGlitchWaitSecondsLeft =  Nc::Math::Lerp(WAIT_RANGE.x, WAIT_RANGE.y, intensity);
    radar.screenGlitchWaitSecondsLeft *= Nc::Math::Lerp(0.2f, 1.2f, radar.stability * 0.01f);

    radar.screenGlitchReversed = randomService.RangeFloat(0.0f, 1.0f) < 0.5f;
}
