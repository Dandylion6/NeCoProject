#include "game/construction/scene/comms_scene/object/radar_object.hpp"

#include <cstdint>

#include "raylib.h"
#include "core/data/font.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/serialization/address_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/shared/mechanical/machine_component.hpp"
#include "game/construction/scene/doorway_scene/object/radar_breaker_object.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"
#include "game/utility/color_palette.hpp"

#ifdef DEBUG_BUILD
#include "core/runtime/entity_helpers.hpp"
#include "game/component/shared/debug/dev_settings_component.hpp"
#endif


Object::Radar::Data Object::Radar::Create(const SceneContext& context) noexcept
{
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/radar_screen.png";
	constexpr char ENTITY_ADDRESS[] = "radar";
	constexpr float ATTRACTION_REDUCTION_PER_SECOND = 0.09f;
	constexpr uint16_t POWER_USAGE = 500u;

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, RadarScene);

	const Texture2D& texture = context.store.GetTexture(FILE_PATH);
	context.registry.emplace<Component::Sprite>(entity, texture);

	context.registry.emplace<Component::Address>(entity, ENTITY_ADDRESS);
	context.registry.emplace<Component::Machine>(entity, ATTRACTION_REDUCTION_PER_SECOND, POWER_USAGE);
	auto& radar = context.registry.emplace<Component::Radar>(entity);

	RadarBreaker::Create(context, entity);

#ifdef DEBUG_BUILD
	const entt::entity debugEntity = entt::get_single<Component::Debug::DevSettings>(context.registry);
	const auto& devSettings = context.registry.get<Component::Debug::DevSettings>(debugEntity);
	ToggleState radarState = devSettings.isRadarActiveOnStart ? On : Off;

	auto& toggle = context.registry.emplace<Component::Action::Toggle>(entity, radarState);
#else
	auto& toggle = context.registry.emplace<Component::Action::Toggle>(entity, Off);
#endif

	const entt::entity path = Path::Create(context);
	const entt::entity artillery = Artillery::Create(context);
	const entt::entity errorWarning = ErrorWarning::Create(context.registry);
	const entt::entity recalibrationText = RecalibrationText::Create(context.registry);
	const entt::entity powerButton = PowerButton::Create(context.registry, radar, toggle);

	return { entity, path, artillery, errorWarning, recalibrationText, powerButton };
}


entt::entity Object::Radar::Path::Create(const SceneContext& context) noexcept
{
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/radar_path.png";
	constexpr uint8_t RADAR_MOVE = 0u;
	constexpr Nc::Vector2f TRAVEL_RANGE = Nc::Vector2f(320.0f, -128.0f);
	constexpr float RADAR_TRAVEL_SECONDS = 10.0f;
	constexpr float RADAR_DELAY_SECONDS = 0.8f;

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::Radar::Path>(entity);

	const Texture2D& texture = context.store.GetTexture(FILE_PATH);
	context.registry.emplace<Component::Sprite>(entity, texture);

	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	auto& transform = context.registry.emplace<Component::Transform>(entity, RadarScene, Nc::Vector2f::Zero(), size);
	auto& collection = context.registry.emplace<Component::TweenCollection>(entity);

	// Linear radar path travel animation
	Tween& travel = collection.tweens.at(RADAR_MOVE);
	Tween::Build(
		travel,
		&transform.position.y,
		TRAVEL_RANGE.x,
		TRAVEL_RANGE.y,
		RADAR_TRAVEL_SECONDS,
		Linear,
		RADAR_DELAY_SECONDS
	);
	travel.onComplete = [&travel]() { Tween::Replay(travel); };
	Tween::Play(travel);

	return entity;
}


entt::entity Object::Radar::Artillery::Create(const SceneContext& context) noexcept
{
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/artillery_target.png";

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::Radar::Artillery>(entity);

	Texture2D texture = context.store.GetTexture(FILE_PATH);
	context.registry.emplace<Component::Sprite>(entity, texture);

	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	context.registry.emplace<Component::Transform>(entity, RadarScene, Nc::Vector2f::Zero(), size, size * 0.5f);

	return entity;
}


entt::entity Object::Radar::ErrorWarning::Create(entt::registry& registry) noexcept
{
	constexpr Nc::Vector2f POSITION = Nc::Vector2f(8.0f, RADAR_BOUNDS.max.y - 8.0f);
	constexpr float BLINK_FADE_SECONDS = 0.4f;
	constexpr float BLINK_DELAY_SECONDS = 0.18f;

	const entt::entity entity = registry.create();

	registry.emplace<Component::Transform>(entity, RadarScene, POSITION);
	registry.emplace<Component::Text>(
		entity,
		"( , )",
		Palette::RADAR_COLOR,
		Nc::Font::WDXL,
		Nc::Font::Size::Tiny,
		Alignment::BottomLeft
	);

	auto& errorWarning = registry.emplace<Component::RadarErrorWarning>(entity);
	auto& collection = registry.emplace<Component::TweenCollection>(entity);

	// Basic blink animation
	Tween& blinkFade = collection.tweens.at(Component::RadarErrorWarning::BlinkFade);
	Tween::Build(blinkFade, &errorWarning.alpha, 1.0f, 0.0f, BLINK_FADE_SECONDS, QuadIn, BLINK_DELAY_SECONDS);
	blinkFade.onComplete = [&blinkFade]() { Tween::Replay(blinkFade); };
	Tween::Play(blinkFade);

	return entity;
}


entt::entity Object::Radar::RecalibrationText::Create(entt::registry& registry) noexcept
{
	constexpr Nc::Vector2f POSITION = RADAR_BOUNDS.max * 0.5f;

	const entt::entity entity = registry.create();

	registry.emplace<Tag::Radar::Recalibration>(entity);
	registry.emplace<Component::Transform>(entity, RadarScene, POSITION);
	registry.emplace<Component::Text>(
		entity,
		"RECALIBRATING",
		Palette::RADAR_COLOR,
		Nc::Font::WDXL,
		Nc::Font::Size::Small,
		Alignment::Center
	);

	return entity;
}


entt::entity Object::Radar::PowerButton::Create(
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
	auto& action = registry.emplace<Component::Action::Click>(entity);
	action.onClick = [&action, &radar, &toggle]()
	{
		// TODO: Add active/inactive visual state change and prevent spamming.
		if (!radar.isRecalibrating)
			toggle.state = Component::Action::Toggle::Next(toggle.state);
		action.isActive = true;
	};

	return entity;
}
