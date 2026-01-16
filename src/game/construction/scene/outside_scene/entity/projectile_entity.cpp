#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/outside_scene/projectile_component.hpp"
#include "game/construction/scene/outside_scene/entity/projectile_entity.hpp"
#include "game/state/scene.hpp"
#include "raylib.h"
#include <utility>


entt::entity Entity::Projectile::Create(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore, 
	Nc::Vector2f hitPosition
) noexcept
{
	constexpr float TRAVEL_TIME = 4.5f;

	entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, Outside, hitPosition);
	registry.emplace<Component::Projectile>(entity, TRAVEL_TIME);

	Sound sound = LoadSoundAlias(resourceStore.GetSound("assets/audio/object/artillery_hit.wav"));
	registry.emplace<Component::Audio>(entity, std::move(sound));

	return entity;
}
