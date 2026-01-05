#pragma once
#include "entt/entity/fwd.hpp"
class Game;
struct GameState;
namespace Nc { class ResourceStore; };


namespace Object
{

class RestartButton final
{
public:
    static void Create(
        Game& game,
        entt::registry& registry,
        Nc::ResourceStore& resourceStore,
        GameState& gameState
    ) noexcept;

};


class RestartToMainButton final
{
public:
    static void Create(
        entt::registry& regsitry,
        Nc::ResourceStore& resourseStore,
        GameState& gameState
    ) noexcept;

};

}
