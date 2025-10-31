#pragma once
#include "entt/entity/fwd.hpp"
class ResourceStore;


namespace Construct
{
	const entt::entity NoteEntity(entt::registry& registry, ResourceStore& resourceStore);
}