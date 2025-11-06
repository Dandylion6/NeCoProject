#pragma once
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Construct
{
	void MorseTransceiverEntity(entt::registry& registry, Nc::ResourceStore& resourceStore);
}