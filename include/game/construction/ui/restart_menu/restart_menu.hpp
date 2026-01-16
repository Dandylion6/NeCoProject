#pragma once
#include "core/data/vector2.hpp"
#include "game/contexts/scene_context.hpp"
#include "entt/entity/fwd.hpp"
class Game;


namespace Structure
{

class RestartMenu final
{
public:
	static void Build(SceneContext context, Game& game, Nc::Vector2f windowSize) noexcept;
	static void Open(SceneContext context) noexcept;
	static void Close(SceneContext context) noexcept;

private:
	static void Toggle(SceneContext context, bool active) noexcept;

};

}
