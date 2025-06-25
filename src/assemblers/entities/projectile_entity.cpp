#include "assemblers/entities/projectile_entity.h"
#include "components/objects/outside/projectile_component.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/vector2.h"


const entt::entity Construct::ProjectileEntity(
	entt::registry& registry, Nc::Vector2f hitPosition
)
{
	const entt::entity entity = registry.create();

	constexpr float TRAVEL_TIME = 4.5f;
	registry.emplace<Component::Projectile>(entity, hitPosition, TRAVEL_TIME);

	return entity;
}