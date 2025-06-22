#include "assemblers/entities/move_region_entity.h"
#include "assemblers/entities/scene_background_entity.h"
#include "assemblers/scenes/comms_scene/comms_scene.h"
#include "assemblers/scenes/comms_scene/morse_transceiver_entity.h"
#include "assemblers/scenes/comms_scene/radio_entity.h"
#include "components/core/rendering/sprite_component.h"
#include "components/core/rendering/text_component.h"
#include "components/core/transform_component.h"
#include "components/core/tween_component.h"
#include "components/objects/radar_tags.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "utility/color.h"
#include "utility/tween.h"
#include "utility/vector2.h"


void CommsScene::Build(
	entt::registry& registry,
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::RadarObject(registry);
	Construct::RadarBlipEntity(registry, Nc::Vector2f(100.0f, 50.0f));

	Construct::RadioEntity(registry);

	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/comms_room.png"), registry, CommsRoom
	);
	Construct::MoveRegionEntity(
		registry, gameState, resourceStore, Down, CommsRoom, CommsDesk, 0.1f
	);
	Construct::MoveRegionEntity(
		registry, gameState, resourceStore, Right, CommsRoom, Doorway, 0.3f
	);

	Construct::MorseTransceiverEntity(registry);
}


void Construct::RadarBlipEntity(entt::registry& registry, Nc::Vector2f position)
{
	const entt::entity entity = registry.create();

	Texture2D texture = LoadTexture("assets/environment/objects/radar/radar_blip.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);
	Nc::Vector2f offset = size * 0.5f;

	constexpr Nc::Hex TEXT_COLOR = 0x7cff3cff;

	registry.emplace<Tag::Blip>(entity);
	registry.emplace<Component::Transform>(entity, Radar, position, size, offset);
	registry.emplace<Component::Text>(entity, "( , )", TEXT_COLOR, WDXL, 16u);

	Component::Sprite& sprite = registry.emplace<Component::Sprite>(entity, texture);
	Component::TweenCollection& tweens = registry.emplace<Component::TweenCollection>(entity);

	sprite.alpha = 0.0f;
	constexpr float FADE_IN_TIME = 0.3f, FADE_OUT_TIME = 6.5f;
	constexpr float FADE_OUT_DELAY = 1.8f;
	
	Tween& fadeInTween = tweens.tweens.at(Tag::Blip::BlipFadeIn);
	Tween& fadeOutTween = tweens.tweens.at(Tag::Blip::BlipFadeOut);

	fadeInTween.Build(&sprite.alpha, sprite.alpha, 1.0f, FADE_IN_TIME, CubicOut, FADE_OUT_DELAY);
	fadeInTween.onComplete = [&fadeOutTween]() { Tween::Replay(fadeOutTween); };
	fadeOutTween.Build(&sprite.alpha, 1.0f, 0.0f, FADE_OUT_TIME, QuadOut);
}