#pragma once
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Construct
{
	const entt::entity MainMenuBackgroundEntity(entt::registry& registry, Nc::ResourceStore& resourceStore);
}