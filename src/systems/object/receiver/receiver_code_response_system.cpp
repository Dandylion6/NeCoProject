#include "components/objects/comms/radio_component.hpp"
#include "components/objects/outside/receiver_component.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/outside/receiver/receiver_code_response_system.hpp"
#include "utility/morse_code.hpp"
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
		switch (receiver.incomingCharacter)
		{
		case MorseCode::NULL_CODE: continue;
		case MorseCode::BACK_CODE: continue;
		case MorseCode::CANCEL_CODE: continue;
		default:
		{
			RespondToCharacterCode(registry, receiver, resourceStore);
			receiver.incomingCharacter = MorseCode::NULL_CODE;
			break;
		}
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
