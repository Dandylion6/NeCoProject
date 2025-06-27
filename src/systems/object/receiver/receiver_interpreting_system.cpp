#include "components/objects/outside/receiver_component.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/outside/receiver/artillery_control_system.h"
#include "systems/object/outside/receiver/artillery_fire_system.h"
#include "systems/object/outside/receiver/receiver_interpreting_system.h"
#include "utility/morse_code.h"
#include <string>
#include <unordered_map>


void ReceiverInterpretingSystem::Update(
	entt::registry& registry,
	ResourceStore& resourceStore
)
{
	auto view = registry.view<Component::Receiver>();
	for (auto [entity, receiver] : view.each())
	{
		if (receiver.incomingCharacter == MorseCode::NULL_CODE) continue;
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
