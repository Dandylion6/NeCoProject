#include "game/system/core/audio/audio_emitter_system.hpp"

#include "raylib.h"
#include "core/data/vector2.hpp"
#include "core/math/interpolation.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"


void System::Audio::Emitter::Update(const SystemContext& context)
{
    // TODO: Apply transition volume logic.
    // TODO: Apply dynamic audio that blends between rooms.
    const auto view = context.registry.view<Component::Transform, Component::Audio>();
    for (auto [entity, transform, emitter] : view.each())
        UpdateEmitter(context.game.currentScene, {context.registry, transform, context.deltaTime, entity}, emitter);

    const auto viewLooped = context.registry.view<Component::Transform, Component::LoopedAudio>();
    for (auto [entity, transform, emitter] : viewLooped.each())
        UpdateLoopedEmitter(context.game.currentScene, {context.registry, transform, context.deltaTime, entity}, emitter);
}


void System::Audio::Emitter::PlayEmitter(const Component::Audio& emitter)
{
    PlaySound(emitter.sound);
}


void System::Audio::Emitter::PlayEmitter(const Component::LoopedAudio& emitter)
{
    if (IsMusicValid(emitter.sound))
        PlayMusicStream(emitter.sound);
}


void System::Audio::Emitter::StopEmitter(const Component::Audio& emitter)
{
    StopSound(emitter.sound);
}


void System::Audio::Emitter::StopEmitter(const Component::LoopedAudio& emitter)
{
    if (IsMusicValid(emitter.sound))
        StopMusicStream(emitter.sound);
}


void System::Audio::Emitter::UpdateEmitter(
    const Scene currentScene,
    const Context& context,
    const Component::Audio& emitter
)
{
    if (!IsSoundPlaying(emitter.sound)) return;

    if (!context.registry.any_of<Component::AudioModifier>(context.entity))
    {
        SetSoundVolume(emitter.sound, emitter.volume);
        return;
    }

    auto& modifier = context.registry.get<Component::AudioModifier>(context.entity);
    const Component::AudioModifier modifierTarget = GetAudioModifier(currentScene, context);

    modifier.stereoPan = Nc::Math::SmoothApproach(
        modifier.stereoPan,
        modifierTarget.stereoPan,
        context.deltaTime,
        Component::AudioModifier::PAN_SPEED
    );
    modifier.volume = Nc::Math::SmoothApproach(
        modifier.volume,
        modifierTarget.volume,
        context.deltaTime,
        Component::AudioModifier::VOLUME_SPEED
    );

    SetSoundVolume(emitter.sound, emitter.volume * modifier.volume);
    SetSoundPan(emitter.sound, modifier.stereoPan);
}


void System::Audio::Emitter::UpdateLoopedEmitter(
    const Scene currentScene,
    const Context& context,
    const Component::LoopedAudio& emitter
)
{
    if (!IsMusicStreamPlaying(emitter.sound)) return;
    UpdateMusicStream(emitter.sound);

    if (!context.registry.any_of<Component::AudioModifier>(context.entity))
    {
        SetMusicVolume(emitter.sound, emitter.volume);
        return;
    }

    auto& modifier = context.registry.get<Component::AudioModifier>(context.entity);
    const Component::AudioModifier modifierTarget = GetAudioModifier(currentScene, context);

    modifier.stereoPan = Nc::Math::SmoothApproach(
        modifier.stereoPan,
        modifierTarget.stereoPan,
        context.deltaTime,
        Component::AudioModifier::PAN_SPEED
    );
    modifier.volume = Nc::Math::SmoothApproach(
        modifier.volume,
        modifierTarget.volume,
        context.deltaTime,
        Component::AudioModifier::VOLUME_SPEED
    );

    SetMusicVolume(emitter.sound, emitter.volume * modifier.volume);
    SetMusicPan(emitter.sound, modifier.stereoPan);
}


Component::AudioModifier System::Audio::Emitter::GetAudioModifier(const Scene currentScene, const Context& context)
{
    constexpr auto WIDTH = static_cast<float>(Nc::RENDER_RESOLUTION.x);

    auto modifier = Component::AudioModifier();

    if (currentScene == context.transform.boundScene)
    {
        const float normalizedX = context.transform.position.x / WIDTH;
        const float stereoPan = Nc::Math::Lerp(1.0f, 0.0f, normalizedX);

        modifier.stereoPan = stereoPan;
        return modifier;
    }

    switch (SceneMath::GetConnectionDirection(currentScene, context.transform.boundScene))
    {
    case None:
        modifier.volume = 0.0f; // Too far.
        return modifier;
    case Up:
    case Down:
        modifier.volume = 0.8f;
        break;
    case Left:
        modifier.stereoPan = 1.0f;
        break;
    case Right:
        modifier.stereoPan = 0.0f;
        break;
    case Front:
        modifier.stereoPan = 0.5f;
        modifier.volume = 0.6f;
        break;
    case Back:
        modifier.stereoPan = 0.5f;
        modifier.volume = 0.4f;
        break;
    }

    return modifier;
}
