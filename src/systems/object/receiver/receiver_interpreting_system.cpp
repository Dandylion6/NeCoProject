#include "components/objects/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/receiver/artillery_control_system.h"
#include "systems/object/receiver/receiver_interpreting_system.h"
#include "utility/morse_code.h"
#include <string>
#include <unordered_map>


void ReceiverInterpretingSystem::Update(entt::registry& registry)
{
	auto view = registry.view<Component::Receiver>();
	for (auto [entity, receiver] : view.each())
	{
		if (receiver.incomingCharacter == MorseCode::NULL_CODE) continue;

		receiver.message += receiver.incomingCharacter;
		TryInterpretMessage(receiver, receiver.message);
	}
}


void ReceiverInterpretingSystem::TryInterpretMessage(
	const Component::Receiver& receiver, const std::string& message
)
{
	TransmissionContext newContext = OnStandby;
	if (receiver.currentContext == OnStandby)
	{
		newContext = TryGetContext(receiver, message);
	}

	switch (newContext)
	{
	case OnStandby:
		break;
	case AimingArtillery:
		ArtilleryControlSystem::CheckReceivedMessage(message);
		break;
	default:
		break;
	}
}


TransmissionContext ReceiverInterpretingSystem::TryGetContext(
	const Component::Receiver& receiver, const std::string& message
)
{
	static const std::unordered_map<std::string, TransmissionContext> simpleCommandMap {
		{ ArtilleryControlSystem::COMMAND, AimingArtillery }
	};

	if (simpleCommandMap.find(message) != simpleCommandMap.end())
	{
		return simpleCommandMap.at(message);
	}

	return OnStandby;
}
