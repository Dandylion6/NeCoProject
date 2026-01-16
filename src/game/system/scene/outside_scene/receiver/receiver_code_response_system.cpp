#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/scene/comms_scene/radio/radio_emitter_system.hpp"
#include "game/system/scene/outside_scene/receiver/receiver_code_response_system.hpp"
#include "game/utility/morse_code.hpp"
#include <format>
#include <string>
#include <string_view>
#include <utility>


void ReceiverCodeResponseSystem::Update(
	entt::registry& registry, Nc::ResourceStore& resourceStore
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
	Nc::ResourceStore& resourceStore
)
{
	constexpr std::string_view FORMAT = "assets/audio/voicelines/receiver/{}_received.wav";
	std::string filePath = std::format(FORMAT, receiver.incomingCharacter);

	Sound response = LoadSoundAlias(resourceStore.GetSound(filePath));
	RadioSoundEmitterSystem::Broadcast(registry, std::move(response), Low);
}
