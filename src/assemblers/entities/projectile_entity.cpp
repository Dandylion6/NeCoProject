#include "assemblers/entities/projectile_entity.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/objects/outside/projectile_component.h"
#include "core/render_context.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.h"
#include <utility>


const entt::entity Construct::ProjectileEntity(
	entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f hitPosition
)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.4f;
	Nc::Vector2f offset = hitPosition - Nc::Vector2f(240.0f, 160.0f);
	position += offset * 0.6f;
	registry.emplace<Component::Transform>(entity, Outside, position);
	
	constexpr float TRAVEL_TIME = 4.5f;
	
	registry.emplace<Component::Projectile>(entity, hitPosition, TRAVEL_TIME);

	Sound sound = LoadSoundAlias(resourceStore.GetSound("assets/audio/object/artillery_hit.wav"));
	registry.emplace<Component::SoundEmitter>(entity, std::move(sound));

	return entity;
}