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
#include "systems/object/comms/radar_render_system.h"
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
	DrawBlips(registry, resourceStore);

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
	const entt::entity entity = registry.view<Tag::Radar>().front();
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);
	Renderer::DrawSprite(sprite, Nc::Vector2f::Zero());
}


void RadarRenderSystem::DrawPath(entt::registry & registry)
{
	const entt::entity entity = registry.view<Tag::RadarPath>().front();
	const Component::Transform& transform = registry.get<const Component::Transform>(entity);
	const Component::Sprite& sprite = registry.get<const Component::Sprite>(entity);
	Renderer::DrawSprite(sprite, transform.position);
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

		constexpr float TEXT_HEIGHT = 16.0f;
		Nc::Vector2f textPosition = transform.position + transform.offset;
		textPosition.y -= TEXT_HEIGHT;

		Renderer::DrawSprite(sprite, transform.position);
		Renderer::DrawText(text, textPosition, resourceStore);
	}
}
