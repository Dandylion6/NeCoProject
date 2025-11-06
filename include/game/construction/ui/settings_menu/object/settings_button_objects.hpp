#pragma once
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
struct Settings;
namespace Nc { class ResourceStore; };


namespace Construct
{
    LabelButton SettingsToMainButton(
        Settings& settings, 
        Settings& pendingSettings, 
        GameState& gameState, 
        entt::registry& registry, 
        Nc::ResourceStore& resourceStore
    );


    LabelButton ApplySettingsButton(
        Settings& settings, 
        Settings& pendingSettings, 
        entt::registry& registry, 
        Nc::ResourceStore& resourceStore
    );
}