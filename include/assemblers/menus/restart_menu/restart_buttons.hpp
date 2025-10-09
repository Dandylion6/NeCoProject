#pragma once
#include "assemblers/ui/label_button_object.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
class ResourceStore;


namespace Construct
{
    LabelButton RestartButton(entt::registry& registry, ResourceStore& resourceStore, GameState& gameState);

    LabelButton RestartToMainButton(entt::registry& regsitry, ResourceStore& resourseStore, GameState& gameState);
}