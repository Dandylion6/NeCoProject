#pragma once
#include "game/contexts/scene_context.hpp"
class Game;


namespace Structure
{

class MainMenu final
{
public:
	// ------ Types ------
	enum ButtonId : uint8_t
	{
		Play,
		Settings,
		Exit,
	};


	// ------ Functions ------
	static void Build(const SceneContext& context, Game& game) noexcept;
	static void Open(const SceneContext& context) noexcept;
	static void Close(const SceneContext& context) noexcept;

};

}
