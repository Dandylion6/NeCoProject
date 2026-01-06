#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/utility/rendering.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/core/rendering/rectangle_render_system.hpp"
#include "core/data/vector2.hpp"


void RectangleRenderSystem::DrawScreen(
	entt::entity entity, entt::registry& registry, Nc::Vector2f cameraPosition
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
	entt::entity entity, entt::registry& registry, Nc::Vector2f windowSize
)
{
	Component::UI::Transform transform = registry.get<Component::UI::Transform>(entity);
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
