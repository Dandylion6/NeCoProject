#pragma once
#include "entt/entity/fwd.hpp"
class ResourceStore;


namespace Construct
{
	void MorseTransceiverEntity(entt::registry& registry, ResourceStore& resourceStore);
}