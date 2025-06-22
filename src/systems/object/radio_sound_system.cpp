#include "components/core/sound_emitter_component.h"
#include "components/objects/radio_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.h"
#include "systems/object/radio_sound_system.h"
#include <utility>


void RadioSoundSystem::Update(entt::registry& registry)
{
	auto view = registry.view<Component::Radio, Component::SoundEmitter>();
	for (auto [entity, radio, emitter] : view.each())
	{
		if (!IsSoundPlaying(emitter.sound)) radio.broadcastPriority = Idle;
	}
}


void RadioSoundSystem::Broadcast(
	entt::registry& registry, 
	Sound&& sound, 
	BroadcastPriority broadcastPriority
)
{
	auto view = registry.view<Component::Radio, Component::SoundEmitter>();
	for (auto [entity, radio, emitter] : view.each())
	{
		//TODO: Improve priority logic
		if (radio.broadcastPriority > broadcastPriority) continue;
		radio.broadcastPriority = broadcastPriority;

		emitter.sound = std::move(sound);
		SoundSystem::PlayEmitter(emitter);
	}
}
