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
        UpdateEmitter(context.game, {context.registry, transform, context.deltaTime, entity}, emitter);

    const auto viewLooped = context.registry.view<Component::Transform, Component::LoopedAudio>();
    for (auto [entity, transform, emitter] : viewLooped.each())
        UpdateLoopedEmitter(context.game, {context.registry, transform, context.deltaTime, entity}, emitter);
}


void System::Audio::Emitter::PlayEmitter(Component::Audio& emitter)
{
    PlaySound(emitter.sound);
    emitter.isPlaying = true;
}


void System::Audio::Emitter::PlayEmitter(Component::LoopedAudio& emitter)
{
    if (IsMusicValid(emitter.sound))
    {
        PlayMusicStream(emitter.sound);
        emitter.isPlaying = true;
    }
}


void System::Audio::Emitter::StopEmitter(Component::Audio& emitter)
{
    StopSound(emitter.sound);
    emitter.isPlaying = false;
}


void System::Audio::Emitter::StopEmitter(Component::LoopedAudio& emitter)
{
    if (IsMusicValid(emitter.sound))
    {
        StopMusicStream(emitter.sound);
        emitter.isPlaying = false;
    }
}


void System::Audio::Emitter::UpdateEmitter(
    const GameState& game,
    const Context& context,
    const Component::Audio& emitter
)
{
    const bool sceneBound = context.transform.boundScene != NullScene;
    if (game.isPaused && sceneBound && IsSoundPlaying(emitter.sound) && emitter.isPlaying)
    {
        StopSound(emitter.sound);
    } else if (!game.isPaused && !IsSoundPlaying(emitter.sound) && emitter.isPlaying)
    {
        PlaySound(emitter.sound);
    }

    if (!emitter.isPlaying) return;

    if (!context.registry.any_of<Component::AudioModifier>(context.entity))
    {
        SetSoundVolume(emitter.sound, emitter.volume);
        return;
    }

    auto& modifier = context.registry.get<Component::AudioModifier>(context.entity);
    const Component::AudioModifier modifierTarget = GetAudioModifier(game, context);

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
    modifier.pitch = Nc::Math::SmoothApproach(
        modifier.pitch,
        modifierTarget.pitch,
        context.deltaTime,
        Component::AudioModifier::PITCH_SPEED
    );

    SetSoundVolume(emitter.sound, emitter.volume * modifier.volume);
    SetSoundPitch(emitter.sound, modifier.pitch);
    SetSoundPan(emitter.sound, modifier.stereoPan);
}


void System::Audio::Emitter::UpdateLoopedEmitter(
    const GameState& game,
    const Context& context,
    const Component::LoopedAudio& emitter
)
{
    const bool sceneBound = context.transform.boundScene != NullScene;
    if (game.isPaused && sceneBound && IsMusicStreamPlaying(emitter.sound) && emitter.isPlaying)
    {
        StopMusicStream(emitter.sound);
    } else if (!game.isPaused && !IsMusicStreamPlaying(emitter.sound) && emitter.isPlaying)
    {
        PlayMusicStream(emitter.sound);
    }

    if (!emitter.isPlaying) return;

    UpdateMusicStream(emitter.sound);

    if (!context.registry.any_of<Component::AudioModifier>(context.entity))
    {
        SetMusicVolume(emitter.sound, emitter.volume);
        return;
    }

    auto& modifier = context.registry.get<Component::AudioModifier>(context.entity);
    const Component::AudioModifier modifierTarget = GetAudioModifier(game, context);

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
    modifier.pitch = Nc::Math::SmoothApproach(
        modifier.pitch,
        modifierTarget.pitch,
        context.deltaTime,
        Component::AudioModifier::PITCH_SPEED
    );

    SetMusicVolume(emitter.sound, emitter.volume * modifier.volume);
    SetMusicPitch(emitter.sound, modifier.pitch);
    SetMusicPan(emitter.sound, modifier.stereoPan);
}


Component::AudioModifier System::Audio::Emitter::GetAudioModifier(const GameState& game, const Context& context)
{
    constexpr auto WIDTH = static_cast<float>(Nc::RENDER_RESOLUTION.x);

    auto modifier = Component::AudioModifier();

    Scene currentScene = game.currentScene;
    if (game.movingToScene != NullScene) currentScene = game.movingToScene;

    if (currentScene == context.transform.boundScene)
    {
        const float normalizedX = context.transform.position.x / WIDTH;
        const float stereoPan =  1.0f - normalizedX;

        modifier.stereoPan = stereoPan;
        modifier.pitch = 1.0f;
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
        modifier.volume = 0.6f;
        modifier.pitch = 0.98f;
        break;
    case Right:
        modifier.stereoPan = 0.0f;
        modifier.volume = 0.6f;
        modifier.pitch = 0.98f;
        break;
    case Front:
        modifier.stereoPan = 0.5f;
        modifier.volume = 0.6f;
        break;
    case Back:
        modifier.stereoPan = 0.5f;
        modifier.volume = 0.4f;
        modifier.pitch = 0.95f;
        break;
    }

    return modifier;
}
