#include "components/core/rendering/sprite_component.hpp"
#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "components/objects/comms/radar.hpp"
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


void RadarRenderSystem::DrawRenderTexture(
	entt::registry& registry,
	const RenderTexture2D& radarRenderTexture,
	Scene currentScene,
	ResourceStore& resourceStore
)
{
	if (currentScene != CommsRoom) return;

	BeginTextureMode(radarRenderTexture);
	BeginBlendMode(BLEND_ADDITIVE);

	ClearBackground(BLANK);

	DrawScreen(registry);
	DrawPath(registry);
	DrawRadarArtillery(registry);
	DrawBlips(registry, resourceStore);
	auto view = registry.view<Component::Transform>();

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


void RadarRenderSystem::DrawScreen(entt::registry& registry)
{
	auto view = registry.view<Component::RadarMachine, const Component::Sprite>();
	for (auto [entity, machine, sprite] : view.each())
	{
		Renderer::DrawSprite(sprite, Nc::Vector2f::Zero());
	}
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
	
	auto blipView = registry.view<Component::Blip, const Component::Transform, const Component::Sprite, Component::Text>();
	for (auto [entity, blip, transform, sprite, text] : blipView.each())
	{
		
		Nc::Vector2f position = Nc::Vector2f::Remap(GameState::WORLD_BOUNDS, RenderContext::RADAR_BOUNDS, transform.position);
		Renderer::DrawSprite(sprite, position, transform.offset, transform.rotation);
		
		Nc::Vector2f textPosition = position + TEXT_OFFSET;
		Nc::Vector2f offset = Renderer::GetTextOffset(text, resourceStore);
		
		text.color.SetAlpha(sprite.alpha);
		Renderer::DrawText(text, textPosition, offset, resourceStore);
	}
}
