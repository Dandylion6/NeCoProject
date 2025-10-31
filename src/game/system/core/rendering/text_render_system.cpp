#include "components/core/rendering/text_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/rendering.hpp"
#include "core/resource_store.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/text_render_system.hpp"
#include "utility/vector2.hpp"


void TextRenderSystem::DrawScreen(
	const entt::entity entity, entt::registry& registry, Nc::Vector2f cameraPosition
)
{
	
};


void TextRenderSystem::DrawUi(
	const entt::entity entity, entt::registry& registry, ResourceStore& resourceStore, Nc::Vector2f windowSize
)
{
	Component::UiTransform& transform = registry.get<Component::UiTransform>(entity);
	const Component::Text& text = registry.get<const Component::Text>(entity);

	Nc::Vector2f anchorPoint = transform.anchor * windowSize;
	Nc::Vector2f position = anchorPoint + transform.offset;

	Renderer::DrawText(text, position, Renderer::GetTextOffset(text, resourceStore), resourceStore);
};