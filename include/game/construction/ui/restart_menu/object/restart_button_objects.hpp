#pragma once
#include "game/construction/ui/shared/object/label_button_object.hpp"
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
namespace Nc { class ResourceStore; };


namespace Construct
{
    LabelButton RestartButton(
        Game& game, entt::registry& registry, Nc::ResourceStore& resourceStore, GameState& gameState
    );

    LabelButton RestartToMainButton(entt::registry& regsitry, Nc::ResourceStore& resourseStore, GameState& gameState);
}