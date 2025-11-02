#include "game/construction/scene/outside_scene/entity/projectile_entity.hpp"
#include "game/component/core/audio/sound_emitter_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/outside_scene/projectile_component.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "core/data/vector2.hpp"
#include <utility>


const entt::entity Construct::ProjectileEntity(
	entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f hitPosition
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = Nc::RENDER_RESOLUTION * 0.4f;
	Nc::Vector2f offset = hitPosition - Nc::Vector2f(240.0f, 160.0f);
	position += offset * 0.6f;
	registry.emplace<Component::Transform>(entity, Outside, position);
	
	constexpr float TRAVEL_TIME = 4.5f;
	
	registry.emplace<Component::Projectile>(entity, hitPosition, TRAVEL_TIME);

	Sound sound = LoadSoundAlias(resourceStore.GetSound("assets/audio/object/artillery_hit.wav"));
	registry.emplace<Component::SoundEmitter>(entity, std::move(sound));

	return entity;
}