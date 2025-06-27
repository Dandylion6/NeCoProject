#include "assemblers/scenes/comms_scene/radar_object.h"
#include "components/core/rendering/sprite_component.h"
#include "components/core/rendering/text_component.h"
#include "components/core/transform_component.h"
#include "components/core/tween_component.h"
#include "components/objects/comms/radar_tags.h"
#include "components/objects/health_component.h"
#include "components/objects/outside/blip_component.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/color.h"
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
	Construct::RadarArtilleryEntity(registry);
}


const entt::entity Construct::RadarPathEntity(entt::registry& registry)
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


const entt::entity Construct::RadarArtilleryEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Texture2D texture = LoadTexture("assets/environment/objects/radar/artillery_target.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	registry.emplace<Tag::RadarArtillery>(entity);
	registry.emplace<Component::Transform>(entity, Radar, Nc::Vector2f(40.0f, 140.0f), size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, texture);

	return entity;
}


const entt::entity Construct::RadarBlipEntity(
	entt::registry& registry, Nc::Vector2f position, int16_t health
)
{
	const entt::entity entity = registry.create();

	Texture2D texture = LoadTexture("assets/environment/objects/radar/radar_blip.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	constexpr Nc::Hex TEXT_COLOR = 0x7cff3cff;

	registry.emplace<Component::Blip>(entity);
	registry.emplace<Component::Health>(entity, health);
	registry.emplace<Component::Transform>(entity, Radar, position, size, size * 0.5f);
	registry.emplace<Component::Text>(entity, "( , )", TEXT_COLOR, WDXL, 16u);

	Component::Sprite& sprite = registry.emplace<Component::Sprite>(entity, texture);
	Component::TweenCollection& tweens = registry.emplace<Component::TweenCollection>(entity);

	sprite.alpha = 0.0f;
	constexpr float FADE_IN_TIME = 0.3f, FADE_OUT_TIME = 6.5f;
	constexpr float FADE_OUT_DELAY = 1.8f;

	Tween& fadeInTween = tweens.tweens.at(Component::Blip::BlipFadeIn);
	Tween& fadeOutTween = tweens.tweens.at(Component::Blip::BlipFadeOut);

	fadeInTween.Build(&sprite.alpha, sprite.alpha, 1.0f, FADE_IN_TIME, CubicOut, FADE_OUT_DELAY);
	fadeInTween.onComplete = [&fadeOutTween]() { Tween::Replay(fadeOutTween); };
	fadeOutTween.Build(&sprite.alpha, 1.0f, 0.0f, FADE_OUT_TIME, QuadOut);

	return entity;
}