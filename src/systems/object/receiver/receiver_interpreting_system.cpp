#include "components/objects/comms/radio_component.hpp"
#include "components/objects/outside/receiver_component.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "systems/object/comms/radio_sound_system.hpp"
#include "systems/object/outside/receiver/artillery_control_system.hpp"
#include "systems/object/outside/receiver/artillery_fire_system.hpp"
#include "systems/object/outside/receiver/receiver_interpreting_system.hpp"
#include "utility/morse_code.hpp"
#include <string>
#include <unordered_map>
#include <utility>
#ifdef DEBUG_BUILD
#include "core/debug_context.hpp"
#include "core/game.hpp"
#endif


void ReceiverInterpretingSystem::Update(
	entt::registry& registry,
	ResourceStore& resourceStore
)
{
	auto view = registry.view<Component::Receiver>();
	for (auto [entity, receiver] : view.each())
	{
#ifdef DEBUG_BUILD
		Game::debugContext.receiverMessage = receiver.message;

		if (IsKeyPressed(KEY_SLASH))
		{
			receiver.message.clear();
			Game::debugContext.receiverMessage.clear();
			continue;
		}
#endif

		switch (receiver.incomingCharacter)
		{
		case MorseCode::NULL_CODE:
			continue;
		case MorseCode::BACK_CODE:
		{
			if (receiver.message.empty()) continue;
			receiver.incomingCharacter = MorseCode::NULL_CODE;
			receiver.message.pop_back();

			const std::string RESPONSE = "assets/audio/voicelines/receiver/commands/remove_request.wav";
			Sound response = LoadSoundAlias(resourceStore.GetSound(RESPONSE));
			RadioSoundSystem::Broadcast(registry, std::move(response), Low);
			continue;
		}
		case MorseCode::CANCEL_CODE:
		{
			receiver.incomingCharacter = MorseCode::NULL_CODE;
			receiver.message.clear();

			const std::string RESPONSE = "assets/audio/voicelines/receiver/commands/clear_transmission_request.wav";
			Sound response = LoadSoundAlias(resourceStore.GetSound(RESPONSE));
			RadioSoundSystem::Broadcast(registry, std::move(response), Low);
			continue;
		}
		default:
			break;
		}

		receiver.message += receiver.incomingCharacter;
		TryInterpretMessage(registry, resourceStore, receiver, receiver.message);
	}
}


void ReceiverInterpretingSystem::TryInterpretMessage(
	entt::registry& registry,
	ResourceStore& resourceStore,
	Component::Receiver& receiver, 
	const std::string& message
)
{
	TransmissionContext newContext = TryGetContext(receiver, message);
	if (newContext != OnStandby) receiver.currentContext = newContext;

	switch (receiver.currentContext)
	{
	case OnStandby:
		break;
	case AimingArtillery:
		return ArtilleryControlSystem::HandleReceivedMessage(
			registry, resourceStore, receiver, message
		);
	case FiringArtillery:
		return ArtilleryFireSystem::HandleReceivedMessage(
			registry, resourceStore, receiver, message
		);
	default:
		break;
	}
}


TransmissionContext ReceiverInterpretingSystem::TryGetContext(
	const Component::Receiver& receiver, const std::string& message
)
{
	static const auto commandMap = []
	{
		std::unordered_map<std::string, TransmissionContext> map;
		map[ArtilleryControlSystem::COMMAND] = AimingArtillery;
		map[ArtilleryFireSystem::COMMAND] = FiringArtillery;
		return map;
	}();

	if (commandMap.find(message) == commandMap.end()) return OnStandby;
	return commandMap.at(message);
}
