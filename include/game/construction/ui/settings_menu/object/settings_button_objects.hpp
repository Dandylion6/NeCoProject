#pragma once
#include "entt/entity/fwd.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
struct GameState;
struct Settings;
namespace Nc { class ResourceStore; };


namespace Object
{

class SettingsToMainButton final
{
public:
    static void Create(
        entt::registry& registry, 
        Nc::ResourceStore& resourceStore,
        Settings& settings, 
        Settings& pendingSettings, 
        GameState& gameState
    ) noexcept;

};


class ApplySettingsButton final
{
public:
    static void Create(
        entt::registry& registry, 
        Nc::ResourceStore& resourceStore,
        Settings& settings, 
        Settings& pendingSettings
    ) noexcept;

};

}