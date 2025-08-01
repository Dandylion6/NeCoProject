#pragma once
#include "assemblers/ui/label_button_object.hpp"
#include "core/settings.hpp"
#include "entt/entity/fwd.hpp"
class GameState;
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