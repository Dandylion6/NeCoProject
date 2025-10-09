#include "assemblers/menus/restart_menu/restart_buttons.hpp"
#include "assemblers/menus/restart_menu/restart_menu.hpp"
#include "assemblers/ui/label_button_object.hpp"
#include "assemblers/scenes/main_menu/main_menu.hpp"
#include "components/scene/dont_destroy_on_load_tag.hpp"
#include "components/ui/restart_menu_tag.hpp"
#include "core/game.hpp"
#include "core/save_game.hpp"


LabelButton Construct::RestartButton(Game& game, entt::registry& registry, ResourceStore& resourceStore, GameState& gameState)
{
    Component::UiTransform transform = Component::UiTransform(
        Nc::Vector2f(0.5f, 0.5f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> toMainMenu = [&game, &registry, &gameState]()
    {
        RestartMenu::Close(registry, gameState);
        Save::LoadGame(game, registry, gameState);
    };

    return Construct::LabelButtonObject<Tag::RestartMenu, Tag::DontDestroyOnLoad>(std::move(transform), "RESTART FROM SAVE", std::move(toMainMenu), registry, resourceStore);
}


LabelButton Construct::RestartToMainButton(entt::registry& registry, ResourceStore& resourceStore, GameState& gameState)
{
    Component::UiTransform transform = Component::UiTransform(
        Nc::Vector2f(0.5f, 0.56f), Nc::Vector2f::Scale(0.5f), Nc::Vector2f::Zero(), Nc::Vector2f::Zero(), 2
    );
    std::function<void()> toMainMenu = [&registry, &gameState]()
    {
        RestartMenu::Close(registry, gameState);
        MainMenu::Open(registry, gameState);
    };

    return Construct::LabelButtonObject<Tag::RestartMenu, Tag::DontDestroyOnLoad>(std::move(transform), "TO MAIN MENU", std::move(toMainMenu), registry, resourceStore);
}
