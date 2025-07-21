#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/rendering.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/rectangle_render_system.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"


void RectangleRenderSystem::DrawScreen(
	entt::registry& registry, Scene currentScene, Nc::Vector2f cameraPosition
)
{
	auto view = registry.view<const Component::Transform, const Component::Rectangle>();
	for (auto [entity, transform, rectangle] : view.each())
	{
		if (!Renderer::IsRenderableToScreen(transform.boundScene, currentScene)) continue;
		Nc::Vector2f position = transform.position + cameraPosition;
		Renderer::DrawRectangle(
			position, transform.size, rectangle.fillColor, transform.offset, transform.rotation
		);
	}
}


void RectangleRenderSystem::DrawUi(
	entt::registry& registry, Nc::Vector2f windowSize
)
{
	auto view = registry.view<const Component::UiTransform, const Component::Rectangle>();
	for (auto [entity, transform, rectangle] : view.each())
	{
		if (!transform.isVisible) continue;

		Nc::Vector2f anchorPoint = transform.anchor * windowSize;
		Nc::Vector2f position = anchorPoint + transform.offset;
		Nc::Vector2f origin = transform.origin * transform.size;

		Renderer::DrawRectangle(
			position, transform.size, rectangle.fillColor, origin, transform.rotation
		);
	}
}
