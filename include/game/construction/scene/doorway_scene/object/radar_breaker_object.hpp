#pragma once
#include "entt/entity/fwd.hpp"
class ResourceStore;


namespace Construct
{
	void RadarBreakerObject(entt::registry& registry, ResourceStore& resourceStore, const entt::entity radar);
}