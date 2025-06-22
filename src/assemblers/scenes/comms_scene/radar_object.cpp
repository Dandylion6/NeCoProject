#include "assemblers/scenes/comms_scene/radar_object.h"
#include "components/core/rendering/sprite_component.h"
#include "components/core/transform_component.h"
#include "components/core/tween_component.h"
#include "components/objects/radar_tags.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/tween.h"
#include "utility/vector2.h"
#include <cstdint>


void Construct::RadarObject(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Texture2D texture = LoadTexture("assets/environment/objects/radar/radar_screen.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	registry.emplace<Tag::Radar>(entity);
	registry.emplace<Component::Transform>(entity, Radar);
	registry.emplace<Component::Sprite>(entity, texture);

	Construct::RadarPathEntity(registry);
}


entt::entity Construct::RadarPathEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Texture2D texture = LoadTexture("assets/environment/objects/radar/radar_path.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	registry.emplace<Tag::RadarPath>(entity);

	Component::Transform& transform = registry.emplace<Component::Transform>(
		entity, Radar, Nc::Vector2f::Zero(), size
	);
	registry.emplace<Component::Sprite>(entity, texture);

	Component::TweenCollection& tweens = registry.emplace<Component::TweenCollection>(entity);

	constexpr uint8_t RADAR_MOVE = 0u;
	constexpr Nc::Vector2f TRAVEL_RANGE = Nc::Vector2f(320.0f, -128.0f);
	constexpr float RADAR_TRAVEL_TIME = 10.0f;

	Tween& tween = tweens.tweens.at(RADAR_MOVE);
	tween.Build(&transform.position.y, TRAVEL_RANGE.x, TRAVEL_RANGE.y, RADAR_TRAVEL_TIME);
	tween.onComplete = [&tween]() { Tween::Replay(tween); };
	Tween::Play(tween);

	return entity;
}