#include "components/core/rendering/text_component.h"
#include "components/core/transform_component.h"
#include "core/rendering.h"
#include "core/scene.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "systems/core/rendering/text_render_system.h"
#include "utility/vector2.h"


void TextRenderSystem::DrawScreen(
	entt::registry& registry, Scene currentScene, Nc::Vector2f cameraPosition
)
{
	
};


void TextRenderSystem::DrawUi(
	entt::registry& registry, 
	ResourceStore& resourceStore, 
	Nc::Vector2f windowSize
)
{
	auto view = registry.view<const Component::Text, const Component::UiTransform>();
	for (auto [entity, text, transform] : view.each())
	{
		Nc::Vector2f anchorPoint = transform.anchor * windowSize;
		Nc::Vector2f position = anchorPoint + transform.offset;

		Renderer::DrawText(text, position, resourceStore);
	}
};