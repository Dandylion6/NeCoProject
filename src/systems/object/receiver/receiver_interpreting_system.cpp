#include "components/objects/outside/receiver_component.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/object/receiver/artillery_control_system.h"
#include "systems/object/receiver/receiver_interpreting_system.h"
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
	const Component::Receiver& receiver, 
	const std::string& message
)
{
	TransmissionContext context = receiver.currentContext;
	if (receiver.currentContext == OnStandby)
	{
		context = TryGetContext(receiver, message);
	}

	switch (context)
	{
	case OnStandby:
		break;
	case AimingArtillery:
		ArtilleryControlSystem::CheckReceivedMessage(
			registry, resourceStore, message
		);
		break;
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
		return map;
	}();

	if (commandMap.find(message) == commandMap.end()) return OnStandby;
	return commandMap.at(message);
}
