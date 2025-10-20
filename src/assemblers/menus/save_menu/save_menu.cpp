#include "assemblers/menus/save_menu/save_background_entity.hpp"
#include "assemblers/menus/save_menu/save_menu.hpp"
#include "assemblers/menus/save_menu/save_menu_buttons.hpp"
#include "components/core/transform_component.hpp"
#include "components/ui/save_menu_components.hpp"
#include "core/game_state.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/vector2.hpp"


void SaveMenu::Build(
	entt::registry& registry, ResourceStore& resourceStore, SaveContext& saveContext, GameState& gameState, Nc::Vector2f windowSize
)
{
	Construct::SaveMenuBackgroundEntity(registry, resourceStore, windowSize);
}


namespace SaveMenu
{
	static void Toggle(entt::registry& registry, GameState& gameState, bool active, bool loading)
	{
		auto viewElements = registry.view<const Tag::SaveMenu, Component::UiTransform>();
		for (auto [entity, transform] : viewElements.each())
			transform.isVisible = active;

		auto viewSlots = registry.view<Component::SaveMenuSlot>();
	}
}


void SaveMenu::Open(entt::registry& registry, SaveContext& saveContext)
{
}


void SaveMenu::Close(entt::registry& registry)
{
}