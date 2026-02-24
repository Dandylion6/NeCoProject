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
	const auto viewEmitter = context.registry.view<Component::Transform, Component::Audio>();
	for (auto [entity, transform, emitter] : viewEmitter.each())
		UpdateEmitter(context.game.currentScene, { transform, context.deltaTime, entity }, emitter);

	const auto viewLoopedEmitter = context.registry.view<Component::Transform, Component::LoopedAudio>();
	for (auto [entity, transform, emitter] : viewLoopedEmitter.each())
		UpdateLoopedEmitter(context.game.currentScene, { transform, context.deltaTime, entity }, emitter);
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


void System::Audio::Emitter::UpdateEmitter(const Scene currentScene, const Context& context, const Component::Audio& emitter)
{
	if (!IsSoundPlaying(emitter.sound)) return;

    const Modifier modifier = GetAudioModifier(currentScene, context);

	SetSoundVolume(emitter.sound, emitter.volume * modifier.volumeScale);
	SetSoundPan(emitter.sound, modifier.pan);
}


void System::Audio::Emitter::UpdateLoopedEmitter(const Scene currentScene, const Context& context, const Component::LoopedAudio& emitter)
{
	if (!IsMusicStreamPlaying(emitter.sound)) return;
	UpdateMusicStream(emitter.sound);

    const Modifier modifier = GetAudioModifier(currentScene, context);

	SetMusicVolume(emitter.sound, emitter.volume * modifier.volumeScale);
	SetMusicPan(emitter.sound, modifier.pan);
}


System::Audio::Emitter::Modifier System::Audio::Emitter::GetAudioModifier(const Scene currentScene, const Context& context)
{
    constexpr auto WIDTH = static_cast<float>(Nc::RENDER_RESOLUTION.x);

    auto modifier = Modifier();

    if (currentScene == context.transform.boundScene)
    {
        const float normalizedX = context.transform.position.x / WIDTH;
        const float stereoPan = Nc::Math::Lerp(1.0f, 0.0f, normalizedX);

        modifier.pan = stereoPan;
        return modifier;
    }

    switch (SceneMath::GetConnectionDirection(currentScene, context.transform.boundScene))
    {
    case None:
        modifier.volumeScale = 0.0f; // Too far.
        return modifier;
    case Up:
    case Down:
        modifier.volumeScale = 0.8f;
        break;
    case Left:
        modifier.pan = 1.0f;
        break;
    case Right:
        modifier.pan = 0.0f;
        break;
    case Front:
        modifier.pan = 0.5f;
        modifier.volumeScale = 0.6f;
        break;
    case Back:
        modifier.pan = 0.5f;
        modifier.volumeScale = 0.4f;
        break;
    }

    return  modifier;
}