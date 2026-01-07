#include "game/construction/ui/restart_menu/object/restart_button_objects.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/game.hpp"
#include "game/save/save_game.hpp"
#include "game/state/game_state.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"
#include <functional>
#include <utility>


void Object::RestartButton::Create(
    entt::registry& registry, 
    Nc::ResourceStore& resourceStore, 
    Game& game, 
    GameState& gameState
) noexcept 
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.5f, 0.5f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    std::function<void()> toMainMenu = [&game, &registry, &gameState]()
    {
        game.Load();
        Structure::RestartMenu::Close(registry, gameState);
    };
    
    Component::UI::Transform transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    Object::LabelButton::Create(
        registry, 
        resourceStore,
        std::move(transform), 
        "RESTART FROM SAVE", 
        std::move(toMainMenu) 
    );
}


void Object::RestartToMainButton::Create(
    entt::registry& registry, 
    Nc::ResourceStore& resourceStore, 
    GameState& gameState
) noexcept 
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f (0.5f, 0.56f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    std::function<void()> toMainMenu = [&registry, &gameState]()
    {
        Structure::RestartMenu::Close(registry, gameState);
        Structure::MainMenu::Open(registry, gameState);
    };
    
    Component::UI::Transform transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    Object::LabelButton::Create(
        registry, 
        resourceStore,
        std::move(transform), 
        "TO MAIN MENU", 
        std::move(toMainMenu) 
    );
}
