#include "assemblers/scenes/outside_scene/artillery_entity.hpp"
#include "components/core/sound_emitter_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/health_component.hpp"
#include "components/objects/outside/artillery_component.hpp"
#include "components/objects/outside/receiver_component.hpp"
#include "core/context/render_context.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.hpp"
#include <utility>


void Construct::ArtilleryEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;
	registry.emplace<Component::Transform>(entity, Outside, position);

	registry.emplace<Component::Receiver>(entity);
	registry.emplace<Component::Artillery>(entity);
	// Artillery has 3 people, this might be useful in the future
	registry.emplace<Component::Health>(entity, 3);

	Sound sound = LoadSound("assets/audio/object/artillery_fire.wav");
	registry.emplace<Component::SoundEmitter>(entity, std::move(sound));
}