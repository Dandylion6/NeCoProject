#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"

#include "raylib.h"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/system/core/audio/audio_emitter_system.hpp"


void System::Radio::Emitter::Update(const SystemContext& context)
{
	const entt::entity entity = entt::get_single<Component::Radio>(context.registry);
	auto& radio = context.registry.get<Component::Radio>(entity);
	const auto& emitter = context.registry.get<Component::Audio>(entity);

	if (!radio.isSendingBroadcast)
	{
		if (!IsSoundPlaying(emitter.sound))
		{
			radio.priority = BroadcastPriority::Idle;
			UnloadSoundAlias(emitter.sound); // TODO: Add audio pooling system.
		}
		return;
	}

	if (radio.broadcastDelay <= 0.0f)
	{
		// Starts the broadcast
		radio.isSendingBroadcast = false;
		if (IsSoundPlaying(emitter.sound)) Audio::Emitter::StopEmitter(emitter);
		Audio::Emitter::PlayEmitter(emitter);
		return;
	}
	radio.broadcastDelay -= context.deltaTime;
}


void System::Radio::Emitter::Broadcast(entt::registry& registry, const Sound& sound, const BroadcastPriority priority)
{
	constexpr float DELAY_SECONDS = 0.52f;

	const entt::entity entity = entt::get_single<Component::Radio>(registry);
	auto& radio = registry.get<Component::Radio>(entity);
	auto& emitter = registry.get<Component::Audio>(entity);

	//TODO: Add audio effects for broadcast interruptions.
	if (radio.priority >= priority) return;
	radio.priority = priority;

	emitter.sound = sound;
	radio.broadcastDelay = DELAY_SECONDS;
	radio.isSendingBroadcast = true;
}
