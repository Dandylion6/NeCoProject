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


void Object::RestartButton::Create(SceneContext context, Game& game) noexcept 
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.5f, 0.5f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    std::function<void()> toMainMenu = [context, &game]()
    {
        game.Load();
        Structure::RestartMenu::Close(context);
    };
    
    Component::UI::Transform transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    Object::LabelButton::Create(
        context,
        std::move(transform), 
        "RESTART FROM SAVE", 
        std::move(toMainMenu) 
    );
}


void Object::RestartToMainButton::Create(SceneContext context) noexcept 
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f (0.5f, 0.56f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    std::function<void()> toMainMenu = [context]()
    {
        Structure::RestartMenu::Close(context);
        Structure::MainMenu::Open(context);
    };
    
    Component::UI::Transform transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    Object::LabelButton::Create(
        context,
        std::move(transform), 
        "TO MAIN MENU", 
        std::move(toMainMenu) 
    );
}
