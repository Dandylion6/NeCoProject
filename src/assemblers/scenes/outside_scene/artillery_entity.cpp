#include "assemblers/scenes/outside_scene/artillery_entity.h"
#include "components/objects/receiver_component.h"
#include "components/objects/artillery_component.h"
#include "entt/entity/registry.hpp"


void Construct::ArtilleryEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	registry.emplace<Component::Receiver>(entity);
	registry.emplace<Component::Artillery>(entity);
}