#include "core/game.h"
#include <chrono>


int main()
{
	Game game { };
	game.SetupWindow();
	game.SetupRenderContext();

	game.InitialiseAssemblers();

	auto lastUpdateTime = std::chrono::steady_clock::now();
	while (game.ShouldRun())
	{
		auto currentTime = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime).count() * 0.001f;

		game.Update(deltaTime);
		game.UpdateRegistries(deltaTime);
		game.DrawGame();

		lastUpdateTime = currentTime;
	}

	game.Shutdown();
	return 0;
}