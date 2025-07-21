#include "assemblers/scenes/settings_menu/settings_menu.hpp"
#include "assemblers/scenes/settings_menu/settings_background_entity.hpp"
#include "core/render_context.hpp"


void SettingsMenuScene::Build(
    entt::registry& registry, 
    RenderContext& renderContext,
    GameState&gameState, 
    ResourceStore& resourceStore
) 
{
    Construct::SettingsBackgroundEntity(registry, renderContext.windowSize);
};