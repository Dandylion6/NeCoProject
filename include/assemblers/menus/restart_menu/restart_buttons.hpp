#pragma once
#include "assemblers/ui/label_button_object.hpp"
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
class ResourceStore;
struct SaveContext;


namespace Construct
{
    LabelButton RestartButton(
        Game& game, entt::registry& registry, ResourceStore& resourceStore, SaveContext& saveContext, GameState& gameState
    );

    LabelButton RestartToMainButton(entt::registry& regsitry, ResourceStore& resourseStore, GameState& gameState);
}