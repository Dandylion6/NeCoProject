#include "assemblers/entities/scene_background_entity.hpp"
#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "assemblers/scenes/main_menu/menu_background_entity.hpp"
#include "assemblers/scenes/main_menu/menu_button_entities.hpp"
#include "core/resource_store.hpp"
#include "entt/entity/fwd.hpp"


void MainMenuScene::Build(
	entt::registry& registry, 
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::PlayButtonObject(registry, gameState, resourceStore);

	Construct::MainMenuBackgroundEntity(registry);
}