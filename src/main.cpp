#include "core/game.h"
#include <chrono>
#include <memory>


int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>();
	game->SetupWindow();
	game->SetupRenderContext();

	game->InitialiseAssemblers();

	auto lastUpdateTime = std::chrono::steady_clock::now();
	while (game->ShouldRun())
	{
		auto currentTime = std::chrono::steady_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime).count() * 0.001f;

		game->Update(deltaTime);
		game->UpdateRegistries(deltaTime);
		game->DrawGame();

		lastUpdateTime = currentTime;
	}
	return 0;
}