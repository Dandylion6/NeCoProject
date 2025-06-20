 #pragma once
#include "core/game_state.h"
#include "core/render_context.h"
#include "core/resource_store.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"


class Game
{
public:
	bool ShouldRun() const;
	
	void SetupWindow() const;
	void SetupRenderContext();

	void BuildScenes();
	void Shutdown();

	void Update(float deltaTime);
	void UpdateRegistries(float deltaTime);
	void DrawGame();

private:
	entt::registry registry;
	RenderContext renderContext { };
	GameState gameState { };
	ResourceStore resourceStore { };

	void DrawScreen();
	void DrawUi();
};