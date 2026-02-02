#pragma once
#include "entt/entity/fwd.hpp"
#include "game/contexts/scene_context.hpp"
class Game;


namespace Object
{

class RestartButton final
{
public:
    static void Create(const SceneContext& context) noexcept;

};


class RestartToMainButton final
{
public:
    static void Create(const SceneContext& context) noexcept;

};

}
