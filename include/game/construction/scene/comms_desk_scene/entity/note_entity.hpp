#pragma once
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Construct
{
	const entt::entity NoteEntity(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore
	);
}