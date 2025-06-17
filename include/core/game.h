 #pragma once
#include "core/ecs_context.h"
#include "core/game_state.h"
#include "core/render_context.h"
#include "core/resource_store.h"


class Game
{
public:
	bool ShouldRun() const;
	
	void SetupWindow() const;
	void InitRenderContext();
	void BuildScenes();
	void Shutdown();

	void Update(float deltaTime);
	void UpdateRegistries(float deltaTime);
	void DrawGame();

private:
	EcsContext ecsContext { };
	RenderContext renderContext { };
	GameState gameState { };
	ResourceStore resourceStore { };

	void DrawScreen();
	void DrawUi();
};