#include "components/objects/comms/radio_component.h"
#include "components/objects/outside/receiver_component.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.h"
#include "systems/object/receiver/receiver_code_response_system.h"
#include "utility/morse_code.h"
#include <format>
#include <string>
#include <string_view>
#include <utility>


void ReceiverCodeResponseSystem::Update(
	entt::registry& registry, ResourceStore& resourceStore
)
{
	auto view = registry.view<Component::Receiver>();
	for (auto [entity, receiver] : view.each())
	{
		if (receiver.incomingCharacter != MorseCode::NULL_CODE)
		{
			RespondToCharacterCode(registry, receiver, resourceStore);
			receiver.incomingCharacter = MorseCode::NULL_CODE;
		}
	}
}


void ReceiverCodeResponseSystem::RespondToCharacterCode(
	entt::registry& registry,
	Component::Receiver& receiver, 
	ResourceStore& resourceStore
)
{
	constexpr std::string_view FORMAT = "assets/audio/voicelines/receiver/{}_received.wav";
	std::string filePath = std::format(FORMAT, receiver.incomingCharacter);

	Sound response = LoadSoundAlias(resourceStore.GetSound(filePath));
	RadioSoundSystem::Broadcast(registry, std::move(response), Low);
}
