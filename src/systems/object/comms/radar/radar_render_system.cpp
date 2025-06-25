#include "components/core/rendering/sprite_component.h"
#include "components/core/rendering/text_component.h"
#include "components/core/transform_component.h"
#include "components/objects/comms/radar_tags.h"
#include "core/render_context.h"
#include "core/rendering.h"
#include "core/resource_store.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "format"
#include "raylib.h"
#include "systems/object/comms/radar/radar_render_system.h"
#include "utility/vector2.h"
#include <string>


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

	Nc::Vector2f radarSize = RenderContext::RADAR_SIZE;
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
	auto view = registry.view<Tag::Radar, const Component::Sprite>();
	for (auto [entity, sprite] : view.each())
	{
		Renderer::DrawSprite(sprite, Nc::Vector2f::Zero());
	}
}


void RadarRenderSystem::DrawPath(entt::registry & registry)
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
		Renderer::DrawSprite(sprite, transform.position, transform.offset, transform.rotation);
	}
}


void RadarRenderSystem::DrawBlips(
	entt::registry& registry, ResourceStore& resourceStore
)
{
	auto blipView = registry.view<Tag::Blip, const Component::Transform, const Component::Sprite, Component::Text>();
	for (auto [entity, transform, sprite, text] : blipView.each())
	{
		Nc::Vector2i pixelPosition = transform.position;
		text.text = std::format("({} , {})", pixelPosition.x, pixelPosition.y);
		text.color.SetAlpha(sprite.alpha);

		constexpr Nc::Vector2f TEXT_OFFSET = Nc::Vector2f(16.0f, 0.0f);
		Nc::Vector2f textPosition = transform.position - transform.offset;
		textPosition += TEXT_OFFSET;

		Renderer::DrawSprite(sprite, transform.position, transform.offset, transform.rotation);
		Renderer::DrawText(text, textPosition, resourceStore);
	}
}
