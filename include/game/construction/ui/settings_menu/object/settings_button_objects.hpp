#pragma once
#include "assemblers/ui/label_button_object.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
struct Settings;
class ResourceStore;


namespace Construct
{
    LabelButton SettingsToMainButton(
        Settings& settings, 
        Settings& pendingSettings, 
        GameState& gameState, 
        entt::registry& registry, 
        ResourceStore& resourceStore
    );


    LabelButton ApplySettingsButton(
        Settings& settings, 
        Settings& pendingSettings, 
        entt::registry& registry, 
        ResourceStore& resourceStore
    );
}