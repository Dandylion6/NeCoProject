#include "components/core/rendering/sprite_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/comms/radar.hpp"
#include "components/objects/machine.hpp"
#include "components/objects/outside/blip_component.hpp"
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/rendering.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "format"
#include "raylib.h"
#include "systems/object/comms/radar/radar_render_system.hpp"
#include "utility/vector2.hpp"
#include <string>
#include <sstream>


void RadarRenderSystem::DrawRenderTexture(
	entt::registry& registry,
	const RenderTexture2D& radarRenderTexture,
	Scene currentScene,
	ResourceStore& resourceStore
)
{
	if (currentScene != CommsRoom) return;

	auto view = registry.view<Component::Machine, Component::RadarMachine,  Component::Sprite>();

	BeginTextureMode(radarRenderTexture);
	BeginBlendMode(BLEND_ADDITIVE);

	for (auto [entity, machine, radar, sprite] : view.each())
	{
		if (!machine.isActive)
		{
			ClearBackground(BLACK);
			// TODO: Implement machine when inactive rendering.
			continue;
		}

		bool isRecalibrating = radar.recalibrationTimeLeft > 0.0f;
		if (isRecalibrating)
		{
			DrawRecalibratingScreen(registry, resourceStore, radar, sprite);
			continue;
		}

		DrawActiveScreen(registry, resourceStore, radar, sprite);
	}

	EndTextureMode();
	EndBlendMode();
}


void RadarRenderSystem::DrawRadar(
	entt::registry& registry,
	const RenderTexture2D& radarRenderTexture,
	Nc::Vector2f cameraPosition,
	Scene currentScene
)
{
	if (currentScene != CommsRoom) return;

	Nc::Vector2f radarSize = RenderContext::RADAR_BOUNDS.max;
	Nc::Vector2f position = RenderContext::RADAR_POSITION + cameraPosition;

	Rectangle source { 0, 0, radarSize.x, -radarSize.y };
	Rectangle destination { position.x, position.y, radarSize.x, radarSize.y };

	DrawTexturePro(
		radarRenderTexture.texture,
		source,
		destination,
		Nc::Vector2f::Zero(),
		0.0f,
		WHITE
	);
}


void RadarRenderSystem::DrawActiveScreen(
	entt::registry& registry, ResourceStore& resourceStore, Component::RadarMachine& radar, Component::Sprite& sprite
)
{
	const std::string BACKGROUND_FILE = "assets/environment/objects/radar/radar_screen.png";

	ClearBackground(BLANK);
	
	sprite.texture = resourceStore.GetTexture(BACKGROUND_FILE);
	Renderer::DrawSprite(sprite, Nc::Vector2f::Zero());

	DrawPath(registry);
	DrawRadarArtillery(registry);
	DrawBlips(registry, resourceStore);

	bool hasWarnings = radar.glitchCount > 0u;
	if (hasWarnings) 
		DrawErrorWarning(registry, resourceStore, radar);
}


void RadarRenderSystem::DrawPath(entt::registry& registry)
{
	auto view = registry.view<Tag::RadarPath, const Component::Transform, const Component::Sprite>();
	for (auto [entity, transform, sprite] : view.each())
	{
		Renderer::DrawSprite(sprite, transform.position, transform.offset, transform.rotation);
	}
}


void RadarRenderSystem::DrawRadarArtillery(entt::registry& registry)
{
	auto view = registry.view<Tag::RadarArtillery, const Component::Transform, const Component::Sprite>();
	for (auto [entity, transform, sprite] : view.each())
	{
		Nc::Vector2f position = Nc::Vector2f::Remap(GameState::WORLD_BOUNDS, RenderContext::RADAR_BOUNDS, transform.position);
		Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
	}
}


void RadarRenderSystem::DrawBlips(
	entt::registry& registry, ResourceStore& resourceStore
)
{
	constexpr Nc::Vector2f TEXT_OFFSET = Nc::Vector2f::Down(12.0f);
	
	auto view = registry.view<Component::Blip, const Component::Transform, const Component::Sprite, Component::Text>();
	for (auto [entity, blip, transform, sprite, text] : view.each())
	{
		Nc::Vector2f glitchOffset = Nc::Vector2f::Zero();
		if (blip.state == Component::Blip::CompleteFailure)
		{
			Component::Blip::CompleteFailureData& failure = registry.get<Component::Blip::CompleteFailureData>(entity);
			glitchOffset = failure.glitchedOffset;
		}
		
		Nc::Vector2f position = Nc::Vector2f::Remap(GameState::WORLD_BOUNDS, RenderContext::RADAR_BOUNDS, transform.position);
		position += glitchOffset;
		Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
		
		Nc::Vector2f textPosition = position + TEXT_OFFSET;
		Nc::Vector2f offset = Renderer::GetTextOffset(text, resourceStore);
		
		text.color.SetAlpha(sprite.alpha);
		Renderer::DrawText(text, textPosition, offset, resourceStore);
	}
}


void RadarRenderSystem::DrawErrorWarning(
	entt::registry &registry, ResourceStore &resourceStore, Component::RadarMachine& machine
)
{
	auto view = registry.view<const Component::RadarErrorWarning, const Component::Transform, Component::Text>();
	for (auto [entity, errorWarning, transform, text] : view.each())
	{
		std::ostringstream stringStream;
		stringStream << "ERRORS ( " << std::to_string(machine.glitchCount) << " )";
		text.text = stringStream.str();
		text.color.SetAlpha(errorWarning.alpha);

		Nc::Vector2f offset = transform.offset + Renderer::GetTextOffset(text, resourceStore);
		Renderer::DrawText(text, transform.position, offset, resourceStore);
	}
}


void RadarRenderSystem::DrawRecalibratingScreen(
	entt::registry& registry, ResourceStore& resourceStore, Component::RadarMachine& radar, Component::Sprite& sprite
)
{
	constexpr float ANIMATION_SPEED = 6.0f;
	constexpr float BLINK_TIME = 1.4f;
	const std::array<std::string, 6u> loadingStrings = {"[O o o o]", "[o O o o]", "[o o O o]", "[o o o O]", "[o o O o]", "[o O o o]"};

	// TODO: Add custom recalibration background
	ClearBackground(BLACK);
	
	auto view = registry.view<const Tag::RadarRecalibration, const Component::Transform, Component::Text>();
	for (auto [entity, transform, text] : view.each())
	{
		float time = Component::RadarMachine::RECALIBRATION_TIME - radar.recalibrationTimeLeft;
		uint8_t index = static_cast<uint8_t>(time * ANIMATION_SPEED) % loadingStrings.size();
		const std::string& loadingCharacter = loadingStrings.at(index);

		text.color.SetAlpha(0.7f + std::cosf(time * PI * 2.0f / BLINK_TIME) * 0.3f);

		std::ostringstream stringStream;
		stringStream << "RECALIBRATING " << loadingCharacter;
		text.text = stringStream.str();

		Nc::Vector2f offset = transform.offset + Renderer::GetTextOffset(text, resourceStore);
		Renderer::DrawText(text, transform.position, offset, resourceStore);
	}
}