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
	static void Build(const SceneContext& context, Game& game, Nc::Vector2f windowSize) noexcept;
	static void Open(const SceneContext& context) noexcept;
	static void Close(const SceneContext& context) noexcept;

private:
	static void Toggle(const SceneContext& context, bool active) noexcept;

};

}
