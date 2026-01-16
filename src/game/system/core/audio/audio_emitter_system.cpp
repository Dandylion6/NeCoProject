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


void System::Audio::Emitter::Update(const SystemContext& context)
{
	// TODO: Apply transition volume logic.
	// TODO: Apply dynamic audio that blends between rooms.
	const auto viewEmitter = context.registry.view<Component::Transform, Component::Audio>();
	for (auto [entity, transform, emitter] : viewEmitter.each())
		UpdateEmitter({ transform, context.deltaTime, entity }, emitter);

	const auto viewLoopedEmitter = context.registry.view<Component::Transform, Component::LoopedAudio>();
	for (auto [entity, transform, emitter] : viewLoopedEmitter.each())
		UpdateLoopedEmitter({ transform, context.deltaTime, entity }, emitter);
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


void System::Audio::Emitter::UpdateEmitter(const Context& context, const Component::Audio& emitter)
{
	if (!IsSoundPlaying(emitter.sound)) return;

	SetSoundVolume(emitter.sound, emitter.volume);
	SetSoundPan(emitter.sound, GetPan(context.transform.position));
}


void System::Audio::Emitter::UpdateLoopedEmitter(const Context& context, const Component::LoopedAudio& emitter)
{
	if (!IsMusicStreamPlaying(emitter.sound)) return;
	UpdateMusicStream(emitter.sound);

	SetMusicVolume(emitter.sound, emitter.volume);
	SetMusicPan(emitter.sound, GetPan(context.transform.position));
}


float System::Audio::Emitter::GetPan(const Nc::Vector2f position)
{
	constexpr auto WIDTH = static_cast<float>(Nc::RENDER_RESOLUTION.x);

	const float normalizedX = position.x / WIDTH;
	const float stereoPan = Nc::Math::Lerp(1.0f, 0.0f, normalizedX);
	return stereoPan;
}
