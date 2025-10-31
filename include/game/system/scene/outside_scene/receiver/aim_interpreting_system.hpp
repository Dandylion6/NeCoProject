#pragma once
#include "components/objects/outside/receiver_component.hpp"
#include "entt/entity/fwd.hpp"
#include "systems/object/outside/receiver/coordinate_interpreting_system.hpp"
#include <string>
class ResourceStore;


class AimInterpretingSystem
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
	static void ConfirmAimCommand(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Component::Receiver& receiver
	);

	static void SetArtilleryTarget(entt::registry& registry, CoordResult result);

};