#include "game/system/core/interactive/click_action_system.hpp"

#include "raylib.h"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/render_context.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/utility/rendering.hpp"


void System::Action::Click::Update(const SystemContext& context, const Nc::RenderContext& renderContext)
{
    const auto view = context.registry.view<Component::Action::Click>();
    for (auto [entity, click] : view.each())
    {
        click.justClicked = false;
        click.isHeld = false;
        click.justReleased = false;
    }

    const State state = GetClickState();
    UpdateUiButtons(context, renderContext.windowSize, state);

    // Still has no target.
    if (context.game.cursor == Nc::Cursor::Standard)
        UpdateSceneButtons(context, renderContext, state);
}


void System::Action::Click::UpdateSceneButtons(
    const SystemContext& context,
    const Nc::RenderContext& renderContext,
    const State state
)
{
    if (context.game.isPaused) return;

    const auto view = context.registry.view<const Component::Transform, Component::Action::Click>();
    for (auto [entity, transform, button] : view.each())
    {
        if (button.state != Component::Action::Click::Active) continue;
        if (context.game.currentScene != transform.boundScene) continue;

        const Nc::Vector2f mousePosition = Renderer::GetWorldPosition(renderContext, GetMousePosition());

        auto bounds = Nc::Bounds(transform);
        bounds.min += button.offset;
        bounds.max += button.offset;

        if (!Nc::Bounds::PointInBounds(bounds, mousePosition - renderContext.cameraPosition)) continue;

        switch (state)
        {
        case None: break;
        case JustClicked: button.justClicked = true;
            break;
        case IsHeld:
            Nc::Cursor::AssignIfHigherPriority(context.game.cursor, Nc::Cursor::Clicked);
            button.isHeld = true;
            break;
        case JustReleased: button.justReleased = true;
            break;
        }

        Nc::Cursor::AssignIfHigherPriority(context.game.cursor, Nc::Cursor::Clickable);
        break; // Already a target, no need to check further.
    }
}


void System::Action::Click::UpdateUiButtons(
    const SystemContext& context,
    const Nc::Vector2i windowSize,
    const State state
)
{
    const auto view = context.registry.view<const Component::UI::Transform, Component::Action::Click>();
    for (auto [entity, transform, button] : view.each())
    {
        UpdateUiButtonState(button, transform.isVisible);
        if (button.state != Component::Action::Click::Active) continue;

        const auto mousePosition = Nc::Vector2f(GetMousePosition());
        const auto bounds = Nc::Bounds(transform, windowSize);

        if (!Nc::Bounds::PointInBounds(bounds, mousePosition)) continue;

        switch (state)
        {
        case None: break;
        case JustClicked:
            button.state = Component::Action::Click::Inactive;
            button.justClicked = true;
            break;
        case IsHeld:
            Nc::Cursor::AssignIfHigherPriority(context.game.cursor, Nc::Cursor::Clicked);
            button.isHeld = true;
            break;
        case JustReleased: button.justReleased = true;
            break;
        }

        Nc::Cursor::AssignIfHigherPriority(context.game.cursor, Nc::Cursor::Clickable);
        break; // Already a target, no need to check further.
    }
}


void System::Action::Click::UpdateUiButtonState(Component::Action::Click& button, const bool isVisible)
{
    switch (button.state)
    {
    case Component::Action::Click::Disabled:
        break;
    case Component::Action::Click::Active:
        if (!isVisible)
            button.state = Component::Action::Click::Inactive;
        break;
    case Component::Action::Click::Inactive:
        if (isVisible)
            button.state = Component::Action::Click::Active;
        break;
    }
}


System::Action::Click::State System::Action::Click::GetClickState()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return JustClicked;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return IsHeld;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) return JustReleased;
    return None;
}
