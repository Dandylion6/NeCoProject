#include "core/data/font.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/serialization/address_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/shared/mechanical/machine_component.hpp"
#include "game/construction/scene/comms_scene/object/radar_object.hpp"
#include "game/construction/scene/doorway_scene/object/radar_breaker_object.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"
#include "game/utility/color_palette.hpp"
#include "raylib.h"
#include <cstdint>
#include <utility>

#ifdef DEBUG_BUILD
#include "game/game.hpp"
#endif


void Object::Radar::Create(entt::registry& registry, Nc::ResourceStore& resourceStore) noexcept
{
	constexpr float ATTRACTION_REDUCTION_PER_SECOND = 0.09f;
	constexpr uint16_t POWER_USAGE = 500u;

	const entt::entity entity = registry.create();

	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/radar_screen.png");
	registry.emplace<Component::Transform>(entity, RadarScene);
	registry.emplace<Component::Sprite>(entity, std::move(texture));
	
	registry.emplace<Component::Address>(entity, "radar");
	registry.emplace<Component::Machine>(entity, ATTRACTION_REDUCTION_PER_SECOND, POWER_USAGE);
	Component::Radar& radar = registry.emplace<Component::Radar>(entity);

	Object::RadarBreaker::Create(registry, resourceStore, entity);
	
#ifdef DEBUG_BUILD
	ToggleState radarState = Game::debugContext.isRadarActiveOnStart ? On : Off;
	Component::Action::Toggle& toggle = registry.emplace<Component::Action::Toggle>(entity, radarState);
#else
	Component::Action::Toggle& toggle = registry.emplace<Component::Action::Toggle>(entity, Off);
#endif

	Object::Radar::Path::Create(registry, resourceStore);
	Object::Radar::Artillery::Create(registry, resourceStore);
	Object::Radar::ErrorWarning::Create(registry);
	Object::Radar::RecalibrationText::Create(registry);
	Object::Radar::PowerButton::Create(registry, radar, toggle);
}


const entt::entity Object::Radar::Path::Create(
	entt::registry& registry, 
	Nc::ResourceStore& resourceStore
) noexcept
{
	constexpr uint8_t RADAR_MOVE = 0u;
	constexpr Nc::Vector2f TRAVEL_RANGE = Nc::Vector2f(320.0f, -128.0f);
	constexpr float RADAR_TRAVEL_TIME = 10.0f;

	const entt::entity entity = registry.create();

	registry.emplace<Tag::Radar::Path>(entity);
	
	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/radar_path.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	Component::Transform& transform = registry.emplace<Component::Transform>(
		entity, 
		RadarScene, 
		Nc::Vector2f::Zero(), 
		size
	);
	
	registry.emplace<Component::Sprite>(entity, std::move(texture));

	Component::TweenCollection& tweens = registry.emplace<Component::TweenCollection>(entity);

	Tween& tween = tweens.tweens.at(RADAR_MOVE);
	tween.value = &transform.position.y;
	tween.start = TRAVEL_RANGE.x;
	tween.end = TRAVEL_RANGE.y;
	tween.duration = RADAR_TRAVEL_TIME;
	tween.onComplete = [&tween]() { Tween::Replay(tween); };
	Tween::Play(tween);

	return entity;
}


const entt::entity Object::Radar::Artillery::Create(entt::registry& registry, Nc::ResourceStore& resourceStore) noexcept
{
	const entt::entity entity = registry.create();

	registry.emplace<Tag::Radar::Artillery>(entity);

	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/artillery_target.png");
	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);

	registry.emplace<Component::Transform>(entity, RadarScene, Nc::Vector2f::Zero(), size, size * 0.5f);
	registry.emplace<Component::Sprite>(entity, std::move(texture));

	return entity;
}


const entt::entity Object::Radar::ErrorWarning::Create(entt::registry& registry) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(8.0f, RADAR_BOUNDS.max.y - 8.0f);

	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, RadarScene, POSITION);
	Component::Text& text = registry.emplace<Component::Text>(entity, "( , )", Palette::RADAR_COLOR, Nc::Font::WDXL, Nc::Font::Size::Tiny, Alignment::BottomLeft);
	
	Component::RadarErrorWarning& errorWarning = registry.emplace<Component::RadarErrorWarning>(entity);

	// Basic blink tweening
	Component::TweenCollection& collection = registry.emplace<Component::TweenCollection>(entity);
	Tween& blinkFade = collection.tweens.at(Component::RadarErrorWarning::BlinkFade);

	blinkFade.value = &errorWarning.alpha;
	blinkFade.start = 1.0f;
	blinkFade.end = 0.0f;
	blinkFade.duration = 0.4f;
	blinkFade.easing = QuadIn;
	blinkFade.delayComplete = 0.16f;
	blinkFade.onComplete = [&blinkFade]() { Tween::Replay(blinkFade); };
	Tween::Play(blinkFade);

	return entity;
}


const entt::entity Object::Radar::RecalibrationText::Create(entt::registry& registry) noexcept
{
	constexpr Nc::Vector2f POSITION = RADAR_BOUNDS.max * 0.5f;

	const entt::entity entity = registry.create();

	registry.emplace<Tag::Radar::Recalibration>(entity);

	registry.emplace<Component::Transform>(entity, RadarScene, POSITION);
	Component::Text& text = registry.emplace<Component::Text>(
		entity, 
		"RECALIBRATING", 
		Palette::RADAR_COLOR, 
		Nc::Font::WDXL, 
		Nc::Font::Size::Small, 
		Alignment::Center
	);

	return entity;
}


const entt::entity Object::Radar::PowerButton::Create(
	entt::registry& registry, 
	Component::Radar& radar, 
	Component::Action::Toggle& toggle
) noexcept
{
	// TODO: Replace with proper button graphics and size.
	constexpr Nc::Vector2f POSITION = RADAR_POSITION + Nc::Vector2f(280.0f, 340.0f);
	constexpr Nc::Vector2f SIZE = Nc::Vector2f(20.0f, 20.0f);

	const entt::entity entity = registry.create();

	registry.emplace<Tag::Radar::Button>(entity);

	registry.emplace<Component::Transform>(entity, CommsRoom, POSITION, SIZE, SIZE * 0.5f);
	registry.emplace<Component::Rectangle>(entity, RED);

	// Toggles radar machine
	Component::Action::Click& action = registry.emplace<Component::Action::Click>(entity);
	action.onClick = [&action, &radar, &toggle]()
	{
		// TODO: Add active/inactive visual state change and prevent spamming.
		if (!radar.isRecalibrating)
			toggle.state = Component::Action::Toggle::Next(toggle.state);
		action.isActive = true;
	};

	return entity;
}
