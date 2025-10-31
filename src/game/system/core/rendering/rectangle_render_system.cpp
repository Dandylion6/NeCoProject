#include "components/core/rendering/rectangle_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/rendering.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/rectangle_render_system.hpp"
#include "utility/vector2.hpp"


void RectangleRenderSystem::DrawScreen(
	const entt::entity entity, entt::registry& registry, Nc::Vector2f cameraPosition
)
{
	Component::Transform transform = registry.get<Component::Transform>(entity);
	Component::Rectangle rectangle = registry.get<Component::Rectangle>(entity);

	Nc::Vector2f position = transform.position + cameraPosition;
	Renderer::DrawRectangle(
		position, 
		transform.size, 
		rectangle.fillColor, 
		transform.offset, 
		transform.rotation
	);
}


void RectangleRenderSystem::DrawUi(
	const entt::entity entity, entt::registry& registry, Nc::Vector2f windowSize
)
{
	Component::UiTransform transform = registry.get<Component::UiTransform>(entity);
	Component::Rectangle rectangle = registry.get<Component::Rectangle>(entity);

	Nc::Vector2f anchorPoint = transform.anchor * windowSize;
	Nc::Vector2f position = anchorPoint + transform.offset;
	Nc::Vector2f origin = transform.origin * transform.size;

	Renderer::DrawRectangle(
		position, 
		transform.size, 
		rectangle.fillColor, 
		origin, 
		transform.rotation
	);
}
