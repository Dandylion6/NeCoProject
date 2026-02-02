#include "game/construction/ui/restart_menu/object/restart_button_objects.hpp"

#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/game.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/construction/ui/restart_menu/restart_menu.hpp"
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "game/tag/ui/restart_menu_tag.hpp"


void Object::RestartButton::Create(const SceneContext& context) noexcept
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.5f, 0.5f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    auto transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    const LabelButton::Data data = LabelButton::Create(
        context,
        transform,
        "RESTART FROM SAVE",
        Structure::RestartMenu::Restart
    );

    for (const entt::entity entity : data.All())
        context.registry.emplace<Tag::RestartMenu>(entity);
}


void Object::RestartToMainButton::Create(const SceneContext& context) noexcept
{
    constexpr Nc::Vector2f ANCHOR = Nc::Vector2f(0.5f, 0.56f);
    constexpr Nc::Vector2f ORIGIN = Nc::Vector2f::Scale(0.5f);

    auto transform = Component::UI::Transform(ANCHOR, ORIGIN, 2);
    const LabelButton::Data data = LabelButton::Create(
        context,
        transform,
        "TO MAIN MENU",
        Structure::RestartMenu::ToMainMenu
    );

    for (const entt::entity entity : data.All())
        context.registry.emplace<Tag::RestartMenu>(entity);
}
