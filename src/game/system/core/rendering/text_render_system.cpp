#include "game/system/core/rendering/text_render_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/text_component.hpp"
#include "game/utility/rendering.hpp"


void TextRenderSystem::DrawScreen(
	entt::entity entity,
	entt::registry& registry,
	Nc::Vector2f cameraPosition
)
{
	// TODO: Add rendering text to in-game render texture.
}


void TextRenderSystem::DrawUi(
	const entt::entity entity,
	entt::registry& registry,
	Nc::ResourceStore& resourceStore,
	const Nc::Vector2f windowSize
)
{
	const auto& transform = registry.get<Component::UI::Transform>(entity);
	const Component::Text& text = registry.get<const Component::Text>(entity);

	const Nc::Vector2f anchorPoint = Nc::Vector::Modulate(transform.anchor, windowSize);
	const Nc::Vector2f position = anchorPoint + transform.offset;

	Renderer::DrawText(text, position, Renderer::GetTextOffset(text, resourceStore), resourceStore);
};
