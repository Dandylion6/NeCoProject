#include "game/construction/scene/comms_scene/object/radar_object.hpp"
#include "game/construction/scene/doorway_scene/object/radar_breaker_object.hpp"
#include "game/component/core/interactive/button_action_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/shared/stat/health_component.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/shared/mechanical/machine_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/core/serialization/address_component.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "raylib.h"
#include "game/utility/color_palette.hpp"
#include "core/data/tween.hpp"
#include "core/data/vector2.hpp"
#include <cstdint>
#include <utility>

#ifdef DEBUG_BUILD
#include "game/game.hpp"
#endif


namespace Construct
{
	static const entt::entity RadarPathEntity(entt::registry& registry, ResourceStore& resourceStore)
	{
		const entt::entity entity = registry.create();

		Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/radar_path.png");
		Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

		registry.emplace<Tag::Radar::Path>(entity);

		Component::Transform& transform = registry.emplace<Component::Transform>(
			entity, Radar, Nc::Vector2f::Zero(), size
		);
		registry.emplace<Component::Sprite>(entity, std::move(texture));

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


	static const entt::entity RadarArtilleryEntity(entt::registry& registry, ResourceStore& resourceStore)
	{
		const entt::entity entity = registry.create();

		Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/artillery_target.png");
		Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

		registry.emplace<Tag::Radar::Artillery>(entity);
		registry.emplace<Component::Transform>(entity, Radar, Nc::Vector2f::Zero(), size, size * 0.5f);
		registry.emplace<Component::Sprite>(entity, std::move(texture));

		return entity;
	}


	static const entt::entity RadarErrorWarningEntity(entt::registry& registry)
	{
		constexpr Nc::Vector2f POSITION = Nc::Vector2f(8.0f, RADAR_BOUNDS.max.y - 8.0f);

		const entt::entity entity = registry.create();

		Component::RadarErrorWarning& errorWarning = registry.emplace<Component::RadarErrorWarning>(entity);
		registry.emplace<Component::Transform>(entity, Radar, POSITION);
		Component::Text& text = registry.emplace<Component::Text>(entity, "( , )", Palette::RADAR_COLOR, WDXL, FontSize::Tiny, Alignment::BottomLeft);

		// Basic blink tweening
		Component::TweenCollection& collection = registry.emplace<Component::TweenCollection>(entity);
		Tween& blinkFade = collection.tweens.at(Component::RadarErrorWarning::BlinkFade);

		blinkFade.Build(&errorWarning.alpha, 1.0f, 0.0f, 0.4f, QuadIn, 0.16f);
		blinkFade.onComplete = [&blinkFade]() { Tween::Replay(blinkFade); };
		Tween::Play(blinkFade);

		return entity;
	}


	static const entt::entity RadarRecalibrationTextEntity(entt::registry& registry)
	{
		constexpr Nc::Vector2f POSITION = RADAR_BOUNDS.max * 0.5f;

		const entt::entity entity = registry.create();
		registry.emplace<Tag::Radar::Recalibration>(entity);

		registry.emplace<Component::Transform>(entity, Radar, POSITION);
		Component::Text& text = registry.emplace<Component::Text>(entity, "RECALIBRATING", Palette::RADAR_COLOR, WDXL, FontSize::Small, Alignment::Center);

		return entity;
	}


	static const entt::entity RadarPowerButtonEntity(
		entt::registry& registry, Component::Radar& radar, Component::Toggle& toggle
	)
	{
		// TODO: Replace with proper button graphics and size.
		constexpr Nc::Vector2f POSITION = RADAR_POSITION + Nc::Vector2f(280.0f, 340.0f);
		constexpr Nc::Vector2f SIZE = Nc::Vector2f(20.0f, 20.0f);

		const entt::entity entity = registry.create();
		registry.emplace<Tag::Radar::Button>(entity);

		registry.emplace<Component::Transform>(entity, CommsRoom, POSITION, SIZE, SIZE * 0.5f);
		registry.emplace<Component::Rectangle>(entity, RED);

		// Toggles radar machine
		Component::ButtonAction& action = registry.emplace<Component::ButtonAction>(entity);
		action.onClick = [&action, &radar, &toggle]()
			{
				// TODO: Add active/inactive visual state change and prevent spamming.
				if (!radar.isRecalibrating)
				toggle.state = ToggleLogic::Next(toggle.state);
				action.isActive = true;
			};

		return entity;
	}
}


void Construct::RadarObject(entt::registry& registry, ResourceStore& resourceStore)
{
	constexpr float ATTRACTION_REDUCTION_PER_SECOND = 0.09f;
	constexpr uint16_t POWER_USAGE = 500u;

	const entt::entity entity = registry.create();

	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/radar_screen.png");

	registry.emplace<Component::Transform>(entity, Radar);
	registry.emplace<Component::Sprite>(entity, std::move(texture));
	
	registry.emplace<Component::Address>(entity, "radar");
	registry.emplace<Component::Machine>(entity, ATTRACTION_REDUCTION_PER_SECOND, POWER_USAGE);
	Component::Radar& radar = registry.emplace<Component::Radar>(entity);

	Construct::RadarBreakerObject(registry, resourceStore, entity);
	
#ifdef DEBUG_BUILD
	ToggleState radarState = Game::debugContext.isRadarActiveOnStart ? On : Off;
	Component::Toggle& toggle = registry.emplace<Component::Toggle>(entity, radarState);
#else
	Component::Toggle& toggle = registry.emplace<Component::Toggle>(entity, Off);
#endif

	Construct::RadarPathEntity(registry, resourceStore);
	Construct::RadarArtilleryEntity(registry, resourceStore);
	Construct::RadarErrorWarningEntity(registry);
	Construct::RadarRecalibrationTextEntity(registry);
	Construct::RadarPowerButtonEntity(registry, radar, toggle);
}


const entt::entity Construct::RadarBlipEntity(
	entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f position, int16_t health
)
{
	const entt::entity entity = registry.create();

	Texture2D texture = resourceStore.GetTexture("assets/environment/objects/radar/radar_blip.png");
	Nc::Vector2i size = Nc::Vector2i(texture.width, texture.height);

	registry.emplace<Component::Blip>(entity);
	registry.emplace<Component::Health>(entity, health);
	registry.emplace<Component::Transform>(entity, Radar, position, size, size * 0.5f);
	registry.emplace<Component::Text>(entity, "( , )", Palette::RADAR_COLOR, WDXL, FontSize::Tiny);

	Component::Sprite& sprite = registry.emplace<Component::Sprite>(entity, std::move(texture));
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