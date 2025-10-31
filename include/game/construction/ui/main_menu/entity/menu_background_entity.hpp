#pragma once
#include "entt/entity/fwd.hpp"
class ResourceStore;


namespace Construct
{
	const entt::entity MainMenuBackgroundEntity(entt::registry& registry, ResourceStore& resourceStore);
}