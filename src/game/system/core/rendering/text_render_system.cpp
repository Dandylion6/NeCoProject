#include "game/component/core/rendering/text_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/utility/rendering.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/system/core/rendering/text_render_system.hpp"
#include "core/data/vector2.hpp"


void TextRenderSystem::DrawScreen(
	const entt::entity entity, entt::registry& registry, Nc::Vector2f cameraPosition
)
{
	
};


void TextRenderSystem::DrawUi(
	const entt::entity entity, entt::registry& registry, Nc::ResourceStore& resourceStore, Nc::Vector2f windowSize
)
{
	Component::UI::Transform& transform = registry.get<Component::UI::Transform>(entity);
	const Component::Text& text = registry.get<const Component::Text>(entity);

	Nc::Vector2f anchorPoint = transform.anchor * windowSize;
	Nc::Vector2f position = anchorPoint + transform.offset;

	Renderer::DrawText(text, position, Renderer::GetTextOffset(text, resourceStore), resourceStore);
};