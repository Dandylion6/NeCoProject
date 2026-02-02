#include "game/system/ui/move_transition_system.hpp"

#include "core/runtime/entity_helpers.hpp"
#include "game/component/core/tween_component.hpp"
#include "game/component/core/interactive/click_action_component.hpp"
#include "game/component/shared/input/move_region_component.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/contexts/system_context.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/tag/ui/move_transition_tag.hpp"


void System::UI::MoveTransition::Update(const SystemContext& context)
{
    const entt::entity entity = entt::get_single<Tag::MoveTransition>(context.registry);
    auto& collection = context.registry.get<Component::TweenCollection>(entity);

    TransitionDown(context, collection);
    TransitionUp(context, collection);
}


void System::UI::MoveTransition::StartMoveScene(const SceneContext& context, const float moveTime) noexcept
{
    const entt::entity entity = entt::get_single<Tag::MoveTransition, Component::TweenCollection>(context.registry);

    auto& collection = context.registry.get<Component::TweenCollection>(entity);
    collection.tweens.at(Tag::MoveTransition::TransitionUp).delayComplete = moveTime;

    Nc::Tween& tweenDown = collection.tweens.at(Tag::MoveTransition::TransitionDown);
    Nc::Tween::Play(tweenDown);
}


void System::UI::MoveTransition::InstantTransition(const SceneContext& context, const Scene nextScene) noexcept
{
    const entt::entity entity = entt::get_single<Tag::MoveTransition, Component::TweenCollection>(context.registry);

    context.game.currentScene = nextScene;
    context.game.movingToScene = NullScene;

    auto& collection = context.registry.get<Component::TweenCollection>(entity);
    Nc::Tween& tween = collection.tweens.at(Tag::MoveTransition::TransitionUp);
    Nc::Tween::Play(tween);
}


void System::UI::MoveTransition::TransitionDown(const SystemContext& context, Component::TweenCollection& collection)
{
    const Nc::Tween& tweenDown = collection.tweens.at(Tag::MoveTransition::TransitionDown);
    if (!tweenDown.justCompleted) return;

    context.game.currentScene = context.game.movingToScene;
    context.game.movingToScene = NullScene;

    Nc::Tween& tweenUp = collection.tweens.at(Tag::MoveTransition::TransitionUp);
    Nc::Tween::Play(tweenUp);

    const auto view = context.registry.view<Component::MoveRegion, Component::Action::Click>();
    for (auto [entity, region, click] : view.each())
    {
        if (click.state == Component::Action::Click::Disabled) continue;
        click.state = Component::Action::Click::Inactive;
    }
}


void System::UI::MoveTransition::TransitionUp(
    const SystemContext& context,
    const Component::TweenCollection& collection
)
{
    const Nc::Tween& tweenUp = collection.tweens.at(Tag::MoveTransition::TransitionUp);
    if (!tweenUp.justCompleted) return;

    const auto view = context.registry.view<Component::MoveRegion, Component::Action::Click>();
    for (auto [entity, region, click] : view.each())
    {
        if (click.state == Component::Action::Click::Disabled) continue;
        click.state = Component::Action::Click::Active;
    }
}
