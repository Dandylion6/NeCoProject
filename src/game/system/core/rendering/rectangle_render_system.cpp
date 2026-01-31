#include "game/system/core/rendering/rectangle_render_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/rectangle_component.hpp"
#include "game/utility/rendering.hpp"


void RectangleRenderSystem::DrawScreen(
	const entt::entity entity,
	entt::registry& registry,
	const Nc::Vector2f cameraPosition
)
{
	const Component::Transform transform = registry.get<Component::Transform>(entity);
	const Component::Rectangle rectangle = registry.get<Component::Rectangle>(entity);
	const Nc::Vector2f position = transform.position + cameraPosition;

	Renderer::DrawRectangle(position, transform.size, rectangle.fillColor, transform.offset, transform.rotation);
}


void RectangleRenderSystem::DrawUi(const entt::entity entity, entt::registry& registry, const Nc::Vector2f windowSize)
{
	const auto transform = registry.get<Component::UI::Transform>(entity);
	const auto rectangle = registry.get<Component::Rectangle>(entity);

	const Nc::Vector2f anchorPoint = Nc::Vector::Modulate(transform.anchor, windowSize);
	const Nc::Vector2f position = anchorPoint + transform.offset;
	const Nc::Vector2f origin = Nc::Vector::Modulate(transform.origin, transform.size);

	Renderer::DrawRectangle(position, transform.size, rectangle.fillColor, origin, transform.rotation);
}
