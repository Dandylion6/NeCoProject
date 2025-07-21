#include "assemblers/entities/projectile_entity.hpp"
#include "components/core/sound_emitter_component.hpp"
#include "components/objects/comms/radio_component.hpp"
#include "components/objects/outside/artillery_component.hpp"
#include "components/objects/outside/receiver_component.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/core/sound_system.hpp"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/outside/receiver/artillery_fire_system.hpp"
#include <string>
#include <utility>

const std::string ArtilleryFireSystem::COMMAND = "FIRE";


void ArtilleryFireSystem::HandleReceivedMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
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
		artillery.fireDelay = FIRE_DELAY;
	}
	
	const std::string FIRE_RESPONSE = "assets/audio/voicelines/receiver/commands/fire_request.wav";

	Sound response = LoadSoundAlias(resourceStore.GetSound(FIRE_RESPONSE));
	RadioSoundSystem::Broadcast(registry, std::move(response), Medium);

	receiver.currentContext = OnStandby;
	receiver.message.clear();
};


void ArtilleryFireSystem::Update(
	entt::registry& registry,
	ResourceStore& resourceStore,
	float deltaTime
)
{
	auto view = registry.view<Component::Artillery, Component::SoundEmitter>();
	for (auto [entity, artillery, emitter] : view.each())
	{
		if (!artillery.receivedFireRequest) continue;
		if (!artillery.isReadyToFire) continue;

		if (artillery.fireDelay > 0.0f)
		{
			artillery.fireDelay -= deltaTime;
			continue;
		}

		artillery.receivedFireRequest = false;
		Construct::ProjectileEntity(registry, resourceStore, artillery.aimPosition);
		SoundSystem::PlayEmitter(emitter);
	}
};