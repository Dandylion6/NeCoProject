#pragma once
#include "entt/entity/fwd.hpp"
#include <string>
class ResourceStore;


class ArtilleryControlSystem
{
public:
	static const std::string COMMAND;

	static void CheckReceivedMessage(
		entt::registry& registry,
		ResourceStore& resourceStore,
		const std::string& message
	);

};