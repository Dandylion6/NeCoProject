#include "game/system/core/audio/audio_emitter_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/audio_emitter_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Audio::Emitter::Update(const SystemContext& context)
{
    const auto view = context.registry.view<Component::Transform, Component::AudioEmitter>();
    for (auto [entity, transform, emitter] : view.each())
    {
        constexpr float FADE_SPEED = 8.0f;

        UpdateEmitter(context.game.currentScene, emitter, transform);

        const float newVolume = context.game.movingToScene != NullScene ? 1.0f : 0.0f;
        emitter.volumeFade = Nc::Math::SmoothApproach(emitter.volumeFade, newVolume, context.deltaTime, FADE_SPEED);
    }
}

void System::Audio::Emitter::PlayEmitter(Component::AudioEmitter& emitter)
{
    emitter.state = Component::AudioEmitter::Playing;
    emitter.justPlayed = true;
}


void System::Audio::Emitter::StopEmitter(Component::AudioEmitter& emitter)
{
    emitter.state = Component::AudioEmitter::Idle;
    emitter.justPlayed = false;
}


void System::Audio::Emitter::UpdateEmitter(
    const Scene currentScene,
    Component::AudioEmitter& emitter,
    const Component::Transform& transform
) noexcept
{
    constexpr float SCENE_WIDTH = Nc::RENDER_RESOLUTION.x;

    const float positionPan = 1.0f - transform.position.x / SCENE_WIDTH;

    if (transform.boundScene == currentScene || !emitter.isSpacial)
    {
        emitter.attenuation = 0.0f;
        emitter.stereoPan = positionPan;
        emitter.pitch = 1.0f;
        return;
    }

    switch (SceneMath::GetConnectionDirection(currentScene, transform.boundScene)) {
    case None: emitter.attenuation = 1.0f;
        break;
    case Up:
    case Down:
        emitter.attenuation = 0.12f;
        emitter.stereoPan = Nc::Math::Lerp(0.5f, positionPan, 0.8f);
        emitter.pitch = 0.99f;
        break;
    case Left:
        emitter.attenuation = 0.55f;
        emitter.stereoPan = 1.0f;
        emitter.pitch = 0.95f;
        break;
    case Right:
        emitter.attenuation = 0.55f;
        emitter.stereoPan = 0.0f;
        emitter.pitch = 0.95f;
        break;
    case Front:
        emitter.attenuation = 0.4f;
        emitter.stereoPan = Nc::Math::Lerp(0.5f, positionPan, 0.5f);
        emitter.pitch = 0.97f;
        break;
    case Back:
        emitter.attenuation = 0.6f;
        emitter.stereoPan = Nc::Math::Lerp(0.5f, positionPan, 0.2f);
        emitter.pitch = 0.93f;
        break;
    }
}
