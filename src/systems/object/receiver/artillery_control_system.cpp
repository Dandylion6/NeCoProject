#include "components/objects/comms/radio_component.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.h"
#include "systems/object/receiver/artillery_control_system.h"
#include <string>
#include <utility>


const std::string ArtilleryControlSystem::COMMAND = "AIM";


void ArtilleryControlSystem::CheckReceivedMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
	const std::string& message
)
{
	if (message == COMMAND)
	{
		const std::string AIM_RESPONSE = "assets/audio/voicelines/receiver/commands/aim_request.wav";
		Sound response = LoadSoundAlias(resourceStore.GetSound(AIM_RESPONSE));
		RadioSoundSystem::Broadcast(registry, std::move(response), Medium);
	}
};