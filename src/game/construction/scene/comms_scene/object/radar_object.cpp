#include "game/construction/scene/comms_scene/object/radar_object.hpp"

#include "raylib.h"
#include "core/data/font.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
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
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/display/radar_crt_background.png";
	constexpr char ENTITY_ADDRESS[] = "radar";
	constexpr float ATTRACTION_REDUCTION_PER_SECOND = 0.09f;
	constexpr uint16_t POWER_USAGE = 500u;

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Component::Transform>(entity, RadarScene);

	const Texture2D& texture = context.store.GetTexture(FILE_PATH);
	context.registry.emplace<Component::Sprite>(entity, texture);

	context.registry.emplace<Component::Address>(entity, ENTITY_ADDRESS);
	context.registry.emplace<Component::Machine>(entity, ATTRACTION_REDUCTION_PER_SECOND, POWER_USAGE);
	context.registry.emplace<Component::Radar>(entity);

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
	const entt::entity powerButton = PowerButton::Create(context);
    Screen::Create(context);

	return {entity, path, artillery, errorWarning, recalibrationText, powerButton};
}


entt::entity Object::Radar::Path::Create(const SceneContext& context) noexcept
{
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/display/radar_path.png";
	constexpr Nc::Vector2f TRAVEL_RANGE = Nc::Vector2f(256.0f, -72.0f);
	constexpr float RADAR_TRAVEL_SECONDS = 10.0f;
	constexpr float RADAR_DELAY_SECONDS = 0.8f;

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::Radar::Path>(entity);

	const Texture2D& texture = context.store.GetTexture(FILE_PATH);
	context.registry.emplace<Component::Sprite>(entity, texture);

	Nc::Vector2f size = Nc::Vector2f(texture.width, texture.height);
	auto& transform = context.registry.emplace<Component::Transform>(entity, RadarScene, Nc::Vector2f::Zero(), size, Nc::Vector2f::Up(4.0f));
	auto& collection = context.registry.emplace<Component::TweenCollection>(entity);

	// Linear radar path travel animation
	Nc::Tween& travel = collection.tweens.at(Tag::Radar::Path::MOVE);
	Nc::Tween::Build(
		travel,
		&transform.position.y,
		TRAVEL_RANGE.x,
		TRAVEL_RANGE.y,
		RADAR_TRAVEL_SECONDS,
		Linear,
		RADAR_DELAY_SECONDS
	);
	Nc::Tween::Play(travel);
	return entity;
}


entt::entity Object::Radar::Artillery::Create(const SceneContext& context) noexcept
{
	constexpr char FILE_PATH[] = "assets/environment/objects/radar/display/radar_artillery_marker.png";

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
	constexpr auto POSITION = Nc::Vector2f(RADAR_BOUNDS.max.x * 0.5f, RADAR_BOUNDS.max.y - 24.0f);
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
		Alignment::Center
	);

	auto& errorWarning = registry.emplace<Component::RadarErrorWarning>(entity);
	auto& collection = registry.emplace<Component::TweenCollection>(entity);

	// Basic blink animation
	Nc::Tween& blinkFade = collection.tweens.at(Component::RadarErrorWarning::BlinkFade);
	Nc::Tween::Build(blinkFade, &errorWarning.alpha, 1.0f, 0.0f, BLINK_FADE_SECONDS, QuadIn, BLINK_DELAY_SECONDS);

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


entt::entity Object::Radar::PowerButton::Create(const SceneContext& context) noexcept
{
    constexpr char BUTTON_ALBEDO_PATH[] = "assets/environment/objects/radar/crt_power_button_albedo.png";
    constexpr char BUTTON_NORMAL_PATH[] = "assets/environment/objects/radar/crt_power_button_normal.png";
    constexpr char BUTTON_AO_PATH[] = "assets/environment/objects/radar/crt_power_button_ao.png";
    constexpr auto POSITION = Nc::Vector2f(401.0f, 438.0f);

	const entt::entity entity = context.registry.create();

	context.registry.emplace<Tag::Radar::Button>(entity);

    const Texture2D& albedoTexture = context.store.GetTexture(BUTTON_ALBEDO_PATH);
    const Texture2D& normalTexture = context.store.GetTexture(BUTTON_NORMAL_PATH);
    const Texture2D& aoTexture = context.store.GetTexture(BUTTON_AO_PATH);

	context.registry.emplace<Component::Sprite>(entity, albedoTexture, normalTexture, aoTexture);

    auto size = Nc::Vector2f(albedoTexture.width, albedoTexture.height);
	context.registry.emplace<Component::Transform>(entity, CommsRoom, POSITION, size, size * 0.5f);
	context.registry.emplace<Component::Action::Click>(entity);
	return entity;
}


entt::entity Object::Radar::Screen::Create(const SceneContext& context) noexcept
{
    constexpr char SCREEN_ALBEDO_PATH[] = "assets/environment/objects/radar/radar_crt_screen_albedo.png";
    constexpr char SCREEN_NORMAL_PATH[] = "assets/environment/objects/radar/radar_crt_screen_normal.png";
    constexpr char SCREEN_AO_PATH[] = "assets/environment/objects/radar/radar_crt_screen_ao.png";
    constexpr auto POSITION = Nc::Vector2f(589.0f, 269.0f);

    const entt::entity entity = context.registry.create();

    const Texture2D& albedoTexture = context.store.GetTexture(SCREEN_ALBEDO_PATH);
    const Texture2D& normalTexture = context.store.GetTexture(SCREEN_NORMAL_PATH);
    const Texture2D& aoTexture = context.store.GetTexture(SCREEN_AO_PATH);

    context.registry.emplace<Component::Sprite>(entity, albedoTexture, normalTexture, aoTexture, 0.4f);

    auto size = Nc::Vector2f(albedoTexture.width, albedoTexture.height);
    context.registry.emplace<Component::Transform>(entity, CommsRoom, POSITION, size, size * 0.5f, -1);

    return entity;
}
