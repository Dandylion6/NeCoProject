#include "components/core/sound_emitter_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/context/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.hpp"
#include "utility/interpolation.hpp"
#include "utility/vector2.hpp"


void SoundSystem::Update(entt::registry& registry, float deltaTime)
{
	auto viewEmitter = registry.view<Component::Transform, Component::SoundEmitter>();
	for (auto [entity, transform, emitter] : viewEmitter.each())
	{
		UpdateEmitter(entity, transform, emitter, deltaTime);
	}

	auto viewLoopedEmitter = registry.view<Component::Transform, Component::LoopedSoundEmitter>();
	for (auto [entity, transform, emitter] : viewLoopedEmitter.each())
	{
		UpdateLoopedEmitter(entity, transform, emitter, deltaTime);
	}
}


void SoundSystem::PlayEmitter(
	Component::SoundEmitter& emitter
)
{
	PlaySound(emitter.sound);
}


void SoundSystem::PlayEmitter(
	Component::LoopedSoundEmitter& emitter
)
{
	if (!IsMusicValid(emitter.sound)) return;
	PlayMusicStream(emitter.sound);
}


void SoundSystem::StopEmitter(
	Component::SoundEmitter& emitter
)
{
	StopSound(emitter.sound);
}


void SoundSystem::StopEmitter(
	Component::LoopedSoundEmitter& emitter
)
{
	if (!IsMusicValid(emitter.sound)) return;
	StopMusicStream(emitter.sound);
}


void SoundSystem::UpdateEmitter(
	const entt::entity entity, 
	Component::Transform& transform, 
	Component::SoundEmitter& emitter, 
	float deltaTime
)
{
	if (!IsSoundPlaying(emitter.sound)) return;

	SetSoundVolume(emitter.sound, emitter.volume);
	SetSoundPan(emitter.sound, GetPan(transform.position));
}


void SoundSystem::UpdateLoopedEmitter(
	const entt::entity entity, 
	Component::Transform& transform, 
	Component::LoopedSoundEmitter& emitter,
	float deltaTime
)
{
	if (!IsMusicStreamPlaying(emitter.sound)) return;
	UpdateMusicStream(emitter.sound);

	SetMusicVolume(emitter.sound, emitter.volume);
	SetMusicPan(emitter.sound, GetPan(transform.position));
}


float SoundSystem::GetPan(Nc::Vector2f position)
{
	constexpr float WIDTH = static_cast<float>(RenderContext::DISPLAY_SIZE.x);

	float normalizedX = position.x / WIDTH;
	float stereoPan = Math::Lerp(1.0f, 0.0f, normalizedX);
	return stereoPan;
}