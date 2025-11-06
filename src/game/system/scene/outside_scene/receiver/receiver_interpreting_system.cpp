#include "game/component/scene/comms_scene/radio_component.hpp"
#include "game/component/scene/outside_scene/receiver_component.hpp"
#include "game/debug/debug_context.hpp"
#include "game/game.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/system/scene/comms_scene/radio/radio_sound_system.hpp"
#include "game/system/scene/outside_scene/receiver/adjust_interpreting_system.hpp"
#include "game/system/scene/outside_scene/receiver/aim_interpreting_system.hpp"
#include "game/system/scene/outside_scene/receiver/fire_interpreting_system.hpp"
#include "game/system/scene/outside_scene/receiver/recalibrate_interpreting_system.hpp"
#include "game/system/scene/outside_scene/receiver/receiver_interpreting_system.hpp"
#include "game/utility/morse_code.hpp"
#include <string>
#include <unordered_map>
#include <utility>


void ReceiverInterpretingSystem::Update(entt::registry& registry, Nc::ResourceStore& resourceStore)
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
			RadioSoundEmitterSystem::Broadcast(registry, std::move(response), Low);
			continue;
		}
		case MorseCode::CANCEL_CODE:
		{
			receiver.incomingCharacter = MorseCode::NULL_CODE;
			receiver.message.clear();

			const std::string RESPONSE = "assets/audio/voicelines/receiver/commands/clear_transmission_request.wav";
			Sound response = LoadSoundAlias(resourceStore.GetSound(RESPONSE));
			RadioSoundEmitterSystem::Broadcast(registry, std::move(response), Low);
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
	entt::registry& registry, Nc::ResourceStore& resourceStore, Component::Receiver& receiver, const std::string& message
)
{
	TransmissionContext newContext = TryGetContext(receiver, message);
	if (newContext != OnStandby) receiver.currentContext = newContext;

	switch (receiver.currentContext)
	{
	case OnStandby:
		break;
	case AimingArtillery:
		return AimInterpretingSystem::HandleReceivedMessage(registry, resourceStore, receiver, message);
	case AdjustArtillery:
		return AdjustInterpretingSystem::HandleReceivedMessage(registry, resourceStore, receiver, message);
	case FiringArtillery:
		return FireInterpretingSystem::HandleReceivedMessage(registry, resourceStore, receiver, message);
	case RecalibrateRadar:
		return RecalibrateInterpretingSystem::HandleReceivedMessage(registry, resourceStore, receiver, message);
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
		map[AimInterpretingSystem::COMMAND] = AimingArtillery;
		map[AdjustInterpretingSystem::COMMAND] = AdjustArtillery;
		map[FireInterpretingSystem::COMMAND] = FiringArtillery;
		map[RecalibrateInterpretingSystem::COMMAND] = RecalibrateRadar;
		return map;
	}();

	if (commandMap.find(message) == commandMap.end()) return OnStandby;
	return commandMap.at(message);
}
