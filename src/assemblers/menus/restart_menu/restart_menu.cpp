#include "assemblers/menus/restart_menu/restart_background_entity.hpp"
#include "assemblers/menus/restart_menu/restart_menu.hpp"
#include "entt/entity/registry.hpp"


void RestartMenu::Build(
	entt::registry& registry, GameState& gameState, ResourceStore& resourceStore
)
{
	Construct::RestartMenuBackgroundEntity(registry, gameState, resourceStore);
}


void RestartMenu::Open(entt::registry& registry, GameState& gameState)
{
	
}


void RestartMenu::Close(entt::registry& registry, GameState& gameState)
{

}