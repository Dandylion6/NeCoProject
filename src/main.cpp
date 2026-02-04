#include "game/game.hpp"
#include "raylib.h"
#include <memory>


int main(const int args, char* argv[])
{
	const std::unique_ptr<Game> game = std::make_unique<Game>();

#ifdef DEBUG_BUILD
	game->SetupDebug(args, argv);
#endif // DEBUG_BUILD

	game->SetupWindow();
	game->SetupRenderContext();
	game->BuildMenuUI();

	while (game->ShouldRun())
	{
		const float deltaTime = GetFrameTime();

		game->Update(deltaTime);
		game->UpdateSystems(deltaTime);
		game->DrawGame(deltaTime);
		game->HandleEvents();
	}

	Game::Shutdown();

	return 0;
}