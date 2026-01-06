#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include <cstdint>
namespace Nc { class ResourceStore; };


namespace Entity
{

class Blip final
{
public:
	static entt::entity Create(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore, 
		Nc::Vector2f position, 
		int16_t health = 10
	) noexcept;

};

}