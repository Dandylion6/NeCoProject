#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Entity
{

class Projectile final
{
public:
	static entt::entity Create(
		entt::registry& registry,
		Nc::ResourceStore& resourceStore,
		Nc::Vector2f hitPosition
	) noexcept;

};

}