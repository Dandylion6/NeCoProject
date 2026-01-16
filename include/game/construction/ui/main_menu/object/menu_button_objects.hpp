#pragma once
#include "game/contexts/scene_context.hpp"
class Game;


namespace Object
{

class PlayButton final
{
public:
	// ------ Functions ------
	static void Create(const SceneContext& context, Game& game) noexcept;

};


class SettingsButton final
{
public:
	// ------ Functions ------
	static void Create(const SceneContext& context) noexcept;

};


class ExitButton final
{
public:
	// ------ Functions ------
	static void Create(const SceneContext& context) noexcept;

};

}
