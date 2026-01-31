#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/rendering/sprite_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/comms_scene/blip_components.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/system/scene/comms_scene/radar/radar_render_system.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"
#include "game/utility/rendering.hpp"
#include "raylib.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <string>

#include "core/math/nc_math.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "game/component/core/interactive/drag_action_component.hpp"
#include "game/contexts/system_context.hpp"


void System::Render::Radar::DrawRenderTexture(const SystemContext& context, const RenderTexture2D& radarRenderTexture)
{
	if (context.game.currentScene != CommsRoom) return;

	BeginTextureMode(radarRenderTexture);
	BeginBlendMode(BLEND_ADDITIVE);

	const entt::entity entity = entt::get_single<Component::Radar>(context.registry);
	const auto& toggle = context.registry.get<Component::Action::Toggle>(entity);
	auto& radar = context.registry.get<Component::Radar>(entity);
	auto& sprite = context.registry.get<Component::Sprite>(entity);

	const Context radarContext = {context, radar, sprite};

	// TODO: Add different visual states for on, off, and disabled (broken).
	if (toggle.state != On)
	{
		ClearBackground(BLACK);
		return;
	}

	if (radar.recalibrationTimeLeft > 0.0f)
	{
		DrawRecalibratingScreen(radarContext);
		return;
	}

	DrawActiveScreen(radarContext);

	EndTextureMode();
	EndBlendMode();
}


void System::Render::Radar::DrawRadar(const SystemContext& context,	const RenderTexture2D& radarRenderTexture, const Nc::Vector2f cameraPosition)
{
	constexpr Nc::Vector2f RADAR_SIZE = RADAR_BOUNDS.max;
	constexpr Rectangle SOURCE { 0, 0, RADAR_SIZE.x, -RADAR_SIZE.y };

	if (context.game.currentScene != CommsRoom) return;

	const Nc::Vector2f position = RADAR_POSITION + cameraPosition;
	const Rectangle destination { position.x, position.y, RADAR_SIZE.x, RADAR_SIZE.y };

	DrawTexturePro(radarRenderTexture.texture, SOURCE, destination,Vector2(), 0.0f, WHITE);
}


void System::Render::Radar::DrawActiveScreen(const Context& context)
{
	constexpr char BACKGROUND_FILE[] = "assets/environment/objects/radar/radar_screen.png";

	ClearBackground(BLANK);
	
	context.sprite.texture = context.systemContext.store.GetTexture(BACKGROUND_FILE);
	Renderer::DrawSprite(context.sprite, Nc::Vector2f::Zero());

	DrawPath(context.systemContext.registry);
	DrawRadarArtillery(context.systemContext.registry);
	DrawBlips(context.systemContext);

	if (context.radar.glitchCount > 0u && context.radar.stability <= Component::Radar::HEALTHY_LEVEL)
		DrawErrorWarning(context);
}


void System::Render::Radar::DrawPath(entt::registry& registry)
{
	const entt::entity entity = entt::get_single<Tag::Radar::Path>(registry);
	const auto& transform = registry.get<Component::Transform>(entity);
	const auto& sprite = registry.get<Component::Sprite>(entity);

	Renderer::DrawSprite(sprite, transform.position, transform.offset, transform.rotation);
}


void System::Render::Radar::DrawRadarArtillery(entt::registry& registry)
{
	const entt::entity entity = entt::get_single<Tag::Radar::Artillery>(registry);
	const auto& transform = registry.get<Component::Transform>(entity);
	const auto& sprite = registry.get<Component::Sprite>(entity);

	const Nc::Vector2f position = Nc::Vector::Remap(
			WORLD_BOUNDS.min,
			WORLD_BOUNDS.max,
			RADAR_BOUNDS.min,
			RADAR_BOUNDS.max,
			transform.position
		);

	Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
}


void System::Render::Radar::DrawBlips(const SystemContext& context)
{
	constexpr Nc::Vector2f TEXT_OFFSET = Nc::Vector2f::Down(12.0f);

	const auto view = context.registry.view<Component::Blip, Component::Transform, Component::Sprite, Component::Text>();
	for (auto [entity, blip, transform, sprite, text] : view.each())
	{
		Nc::Vector2f glitchOffset = Nc::Vector2f::Zero();
		if (blip.state == Component::Blip::CompleteFailure)
		{
			const auto& failure = context.registry.get<Component::Glitch::ContactFailure>(entity);
			glitchOffset = failure.glitchedOffset;
		}

		Nc::Vector2f position = Nc::Vector::Remap(
			WORLD_BOUNDS.min,
			WORLD_BOUNDS.max,
			RADAR_BOUNDS.min,
			RADAR_BOUNDS.max,
			transform.position
		);
		position += glitchOffset;
		Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);

		const Nc::Vector2f textPosition = position + TEXT_OFFSET;
		const Nc::Vector2f offset = Renderer::GetTextOffset(text, context.store);

		Nc::RGBa::SetAlphaFor(text.color, sprite.alpha);
		Renderer::DrawText(text, textPosition, offset, context.store);
	}
}


void System::Render::Radar::DrawErrorWarning(const Context& context)
{
	const entt::entity entity = entt::get_single<Component::RadarErrorWarning>(context.systemContext.registry);
	const auto& errorWarning = context.systemContext.registry.get<Component::RadarErrorWarning>(entity);
	const auto& transform = context.systemContext.registry.get<Component::Transform>(entity);
	auto& text = context.systemContext.registry.get<Component::Text>(entity);

	text.text = "ERRORS ( " + std::to_string(context.radar.glitchCount) + " )";
	Nc::RGBa::SetAlphaFor(text.color, errorWarning.alpha);

	const Nc::Vector2f offset = transform.offset + Renderer::GetTextOffset(text, context.systemContext.store);
	Renderer::DrawText(text, transform.position, offset, context.systemContext.store);
}


void System::Render::Radar::DrawRecalibratingScreen(const Context& context)
{
	// TODO: Add custom recalibration background
	ClearBackground(BLACK);

	const entt::entity entity = entt::get_single<Tag::Radar::Recalibration>(context.systemContext.registry);
	const auto& transform = context.systemContext.registry.get<Component::Transform>(entity);
	auto& text = context.systemContext.registry.get<Component::Text>(entity);

	constexpr float BLINK_TIME = 1.4f;

	const float time = Component::Radar::RECALIBRATION_TIME - context.radar.recalibrationTimeLeft;
	const std::string& loadingString = GetRecalibratingText(time);

	const float blink = 0.7f + std::cos(time * Nc::Math::TWO_PI / BLINK_TIME) * 0.3f;
	Nc::RGBa::SetAlphaFor(text.color, blink);

	text.text = "RECALIBRATING " + loadingString;

	const Nc::Vector2f offset = transform.offset + Renderer::GetTextOffset(text, context.systemContext.store);
	Renderer::DrawText(text, transform.position, offset, context.systemContext.store);
}


std::string System::Render::Radar::GetRecalibratingText(const float time)
{
	constexpr float ANIMATION_SPEED = 6.0f;
	constexpr int ANIMATION_COUNT = 4;

	std::string text = "[";
	const int index = static_cast<int>(time * ANIMATION_SPEED) % ANIMATION_COUNT;

	for (int i = 0; i < ANIMATION_COUNT; ++i)
	{
		text += i == index ? "O" : "o";
		if (i < ANIMATION_COUNT - 1)
			text += " ";
	}

	return text + "]";
}
