#pragma once
#include "components/objects/outside/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include "systems/object/outside/receiver/coordinate_interpreting_system.hpp"
#include <string>
class ResourceStore;


class AdjustInterpretingSystem
{
public:
    static const std::string COMMAND;

	static void HandleReceivedMessage(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Component::Receiver& receiver,
		const std::string& message
	);

private:
    static void ConfirmAdjustCommand(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Component::Receiver& receiver
	);

	static void AdjustArtilery(entt::registry& registry, CoordResult result);
    
};