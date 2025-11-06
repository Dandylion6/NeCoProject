#include "game/construction/ui/main_menu/main_menu.hpp"
#include "game/construction/ui/restart_menu/object/restart_button_objects.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/tag/core/life_cycle/dont_destroy_on_load_tag.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"
#include "game/save/save_game.hpp"
#include "game/game.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/state/game_state.hpp"
#include "entt/entity/fwd.hpp"
#include "core/data/vector2.hpp"
#include <functional>
#include <utility>


LabelButton Construct::RestartButton(
    Game& game, entt::registry& registry, Nc::ResourceStore& resourceStore, GameState& gameState
)
{
    Component::UI::Transform transform = Component::UI::Transform(
        Nc::Vector2f(0.5f, 0.5f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> toMainMenu = [&game, &registry, &gameState]()
    {
        RestartMenu::Close(registry, gameState);
        Save::LoadGame(game, registry, gameState);
    };

    return Construct::LabelButtonObject<Tag::RestartMenu, Tag::DontDestroyOnLoad>(std::move(transform), "RESTART FROM SAVE", std::move(toMainMenu), registry, resourceStore);
}


LabelButton Construct::RestartToMainButton(entt::registry& registry, Nc::ResourceStore& resourceStore, GameState& gameState)
{
    Component::UI::Transform transform = Component::UI::Transform(
        Nc::Vector2f(0.5f, 0.56f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> toMainMenu = [&registry, &gameState]()
    {
        RestartMenu::Close(registry, gameState);
        MainMenu::Open(registry, gameState);
    };

    return Construct::LabelButtonObject<Tag::RestartMenu, Tag::DontDestroyOnLoad>(std::move(transform), "TO MAIN MENU", std::move(toMainMenu), registry, resourceStore);
}
