#pragma once
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Construct
{
	void RadarBreakerObject(entt::registry& registry, Nc::ResourceStore& resourceStore, const entt::entity radar);
}