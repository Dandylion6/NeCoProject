#pragma once
#include "components/objects/outside/receiver_component.h"
#include "entt/entity/fwd.hpp"
#include <string>
class ResourceStore;


class ArtilleryFireSystem
{
public:
	static const std::string COMMAND;

	static void HandleReceivedMessage(
		entt::registry& registry,
		ResourceStore& resourceStore,
		Component::Receiver& receiver,
		const std::string& message
	);

};