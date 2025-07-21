#include "core/game.hpp"
#include "raylib.h"
#include <memory>


int main(int args, char* argv[])
{
	std::unique_ptr<Game> game = std::make_unique<Game>();

#ifdef DEBUG_BUILD
	game->SetupDebug(args, argv);
#endif // DEBUG_BUILD

	game->SetupWindow();
	game->SetupRenderContext();
	game->InitialiseAssemblers();

	while (game->ShouldRun())
	{
		float deltaTime = GetFrameTime();

		game->Update(deltaTime);
		game->UpdateRegistries(deltaTime);
		game->DrawGame();
	}

	game->Shutdown();

	return 0;
}