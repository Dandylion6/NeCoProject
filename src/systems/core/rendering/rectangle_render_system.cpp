#include "components/core/rendering/rectangle_component.h"
#include "components/core/transform_component.h"
#include "core/rendering.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/rectangle_render_system.h"
#include "utility/bounds.h"
#include "utility/vector2.h"


void RectangleRenderSystem::DrawScreen(
	entt::registry& registry, Scene currentScene, Nc::Vector2f cameraPosition
)
{
	auto view = registry.view<const Component::Transform, const Component::Rectangle>();
	for (auto [entity, transform, rectangle] : view.each())
	{
		if (!Renderer::IsRenderableToScreen(transform.boundScene, currentScene)) continue;
		Nc::Vector2f position = (transform.position - transform.position) + cameraPosition;
		Renderer::DrawRectangle(position, transform.size, rectangle.fillColor);
	}
}


void RectangleRenderSystem::DrawUi(
	entt::registry& registry, Nc::Vector2i windowSize
)
{
	auto view = registry.view<const Component::UiTransform, const Component::Rectangle>();
	for (auto [entity, transform, rectangle] : view.each())
	{
		Nc::Bounds bounds = Nc::Bounds(transform, windowSize);
		Nc::Vector2f position = bounds.min, size = Nc::Bounds::SizeOf(bounds);
		Renderer::DrawRectangle(position, size, rectangle.fillColor);
	}
}
