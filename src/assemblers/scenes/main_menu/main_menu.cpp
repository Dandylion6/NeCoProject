#include "assemblers/entities/scene_background_entity.h"
#include "assemblers/scenes/main_menu/main_menu.h"
#include "assemblers/scenes/main_menu/menu_background_entity.h"
#include "assemblers/scenes/main_menu/menu_button_entities.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"


void MainMenuScene::Build(
	entt::registry& registry, 
	GameState& gameState,
	ResourceStore& resourceStore
)
{
	Construct::PlayButtonEntity(registry, gameState, resourceStore);

	Construct::MainMenuBackgroundEntity(registry);
}