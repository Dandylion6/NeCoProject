#include "components/core/rendering/sprite_component.h"
#include "components/core/rendering/text_component.h"
#include "components/core/sound_emitter_component.h"
#include "components/core/transform_component.h"
#include "components/core/tween_component.h"
#include "components/objects/morse_transceiver_component.h"
#include "components/tags/radar_tags.h"
#include "core/ecs_context.h"
#include "core/render_context.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "modules/scenes/comms_scene.h"
#include "raylib.h"
#include "systems/core/sound_system.h"
#include "utility/color.h"
#include "utility/entity/move_entities.h"
#include "utility/entity/scene_entities.h"
#include "utility/tween.h"
#include "utility/vector2.h"
#include <cstdint>
#include <utility>


void CommsScene::Build(
	EcsContext& ecsContext,
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::RadarPathEntity(ecsContext.registry);
	Construct::RadarObject(ecsContext.registry);
	Construct::RadarBlipEntity(ecsContext.registry, Nc::Vector2f(100.0f, 50.0f));

	Construct::SceneBackgroundEntity(
		LoadTexture("assets/environment/backgrounds/comms_room.png"), ecsContext.registry, CommsRoom
	);
	Construct::MoveRegionEntity(
		ecsContext, gameState, resourceStore, Down, CommsRoom, CommsDesk, 0.1f
	);
	Construct::MoveRegionEntity(
		ecsContext, gameState, resourceStore, Right, CommsRoom, Doorway, 0.3f
	);

	Construct::MorseTransceiverEntity(ecsContext.registry);
}


void Construct::MorseTransceiverEntity(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Nc::Vector2f position = RenderContext::DISPLAY_SIZE;
	position *= 0.5f;

	registry.emplace<Component::Transform>(entity, CommsRoom, position);
	registry.emplace<Component::MorseTransceiver>(entity);

	Music morseTone = LoadMusicStream("assets/audio/object/morse_tone.wav");
	Component::LoopedSoundEmitter& emitter = registry.emplace<Component::LoopedSoundEmitter>(entity, std::move(morseTone));
	emitter.volume = 0.0f;
	SoundSystem::PlayEmitter(emitter);
}


void Construct::RadarObject(entt::registry& registry)
{
	const entt::entity entity = registry.create();

	Texture2D texture = LoadTexture("assets/environment/objects/radar/radar_screen.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	registry.emplace<Tag::Radar>(entity);
	registry.emplace<Component::Transform>(entity, Radar);
	registry.emplace<Component::Sprite>(entity, texture);

	Construct::RadarPathEntity(registry);

	const entt::entity backgroundEntity = registry.create();
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