#include "game/construction/scene/outside_scene/entity/projectile_entity.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/core/audio/audio_emitter_system.hpp"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"
#include "game/system/scene/outside_scene/receiver/fire_interpreting_system.hpp"
#include <string>
#include <utility>


const std::string FireInterpretingSystem::COMMAND = "FIRE";


void FireInterpretingSystem::HandleReceivedMessage(
	entt::registry& registry,
	Nc::ResourceStore& resourceStore,
	Component::Receiver& receiver,
	const std::string& message
)
{
	//TODO: Add audio response for when not ready.
	auto view = registry.view<Component::Artillery>();
	for (auto [entity, artillery] : view.each())
	{
		constexpr float FIRE_DELAY = 4.6f;

		artillery.receivedFireRequest = true;
		artillery.fireDelaySeconds = FIRE_DELAY;
	}
	
	const std::string FIRE_RESPONSE = "assets/audio/voicelines/receiver/commands/fire_request.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(FIRE_RESPONSE));
	RadioSoundEmitterSystem::Broadcast(registry, std::move(response), Medium);

	receiver.currentContext = OnStandby;
	receiver.message.clear();
};


void FireInterpretingSystem::Update(
	entt::registry& registry,
	Nc::ResourceStore& resourceStore,
	float deltaTime
)
{
	auto view = registry.view<Component::Artillery, Component::Audio>();
	for (auto [entity, artillery, emitter] : view.each())
	{
		if (!artillery.receivedFireRequest) continue;
		if (!artillery.isReadyToFire) continue;

		if (artillery.fireDelaySeconds > 0.0f)
		{
			artillery.fireDelaySeconds -= deltaTime;
			continue;
		}

		artillery.receivedFireRequest = false;
		Construct::ProjectileEntity(registry, resourceStore, artillery.aimPosition);
		SoundEmitterSystem::PlayEmitter(emitter);
	}
};