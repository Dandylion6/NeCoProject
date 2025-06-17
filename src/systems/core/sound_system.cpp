#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "core/render_context.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.h"
#include "utility/interpolation.h"


void SoundSystem::Update(entt::registry& registry, float deltaTime)
{
	auto view = registry.view<Component::Transform, Component::SoundEmitter>();
	for (auto [entity, transform, emitter] : view.each())
	{
		if (!emitter.isPlaying) continue;
		emitter.elapsed += deltaTime;
		
		SetSoundVolume(emitter.sound, emitter.volume);
		float horizontalSpace = transform.position.x / RenderContext::DISPLAY_SIZE.x;
		float pan = Math::Lerp(1.0f, -1.0f, horizontalSpace);
		SetSoundPan(emitter.sound, pan);

		if (emitter.elapsed < emitter.duration) continue;
		emitter.elapsed = 0.0f;
		
		if (emitter.loops) SoundSystem::PlayEmitter(emitter);
		else
		{
			StopSound(emitter.sound);
			emitter.isPlaying = false;
		}
	}
}


void SoundSystem::PlayEmitter(
	Component::SoundEmitter& emitter
)
{
	emitter.isPlaying = true;
	PlaySound(emitter.sound);
}


void SoundSystem::StopEmitter(
	Component::SoundEmitter& emitter
)
{
	emitter.isPlaying = false;
	emitter.elapsed = 0.0f;
	StopSound(emitter.sound);
}
