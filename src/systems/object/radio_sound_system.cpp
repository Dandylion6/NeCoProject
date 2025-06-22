#include "components/core/sound_emitter_component.h"
#include "components/objects/radio_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.h"
#include "systems/object/radio_sound_system.h"
#include <utility>


void RadioSoundSystem::Update(entt::registry& registry, float deltaTime)
{
	auto view = registry.view<Component::Radio, Component::SoundEmitter>();
	for (auto [entity, radio, emitter] : view.each())
	{
		if (radio.isSendingBroadcast)
		{
			if (radio.broadcastDelay > 0.0f)
			{
				radio.broadcastDelay -= deltaTime;
				continue;
			}

			radio.isSendingBroadcast = false;
			if (IsSoundPlaying(emitter.sound)) SoundSystem::StopEmitter(emitter);
			SoundSystem::PlayEmitter(emitter);
			continue;
		}

		if (!IsSoundPlaying(emitter.sound)) 
			radio.priority = Idle;
	}
}


void RadioSoundSystem::Broadcast(
	entt::registry& registry, 
	Sound&& sound, 
	BroadcastPriority priority
)
{
	auto view = registry.view<Component::Radio, Component::SoundEmitter>();
	for (auto [entity, radio, emitter] : view.each())
	{
		//TODO: Improve priority logic
		if (radio.priority >= priority) continue;
		radio.priority = priority;

		constexpr float DELAY_SECONDS = 0.35f;

		emitter.sound = std::move(sound);
		radio.broadcastDelay = DELAY_SECONDS;
		radio.isSendingBroadcast = true;
	}
}
