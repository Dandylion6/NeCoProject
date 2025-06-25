#include "assemblers/scenes/outside_scene/artillery_entity.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/objects/outside/artillery_component.h"
#include "components/objects/outside/receiver_component.h"
#include "core/render_context.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/vector2.h"
#include <utility>


void Construct::ArtilleryEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE * 0.5f;
	registry.emplace<Component::Transform>(entity, Outside, position);

	registry.emplace<Component::Receiver>(entity);
	registry.emplace<Component::Artillery>(entity);

	Sound sound = LoadSound("assets/audio/object/artillery_fire.wav");
	registry.emplace<Component::SoundEmitter>(entity, std::move(sound));
}