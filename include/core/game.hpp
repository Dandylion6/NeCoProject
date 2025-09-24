 #pragma once
#ifdef DEBUG_BUILD
#include "core/debug_context.hpp"
#endif // DEBUG_BUILD
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/resource_store.hpp"
#include "core/settings.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp" 


class Game
{
public:
#ifdef DEBUG_BUILD
	static DebugContext debugContext;
#endif // DEBUG_BUILD

	Game();

	bool ShouldRun() const;
	
	void SetupWindow() const;
	void SetupRenderContext();

#ifdef DEBUG_BUILD
	void SetupDebug(int args, char* argv[]);
#endif // DEBUG_BUILD

	void InitialiseAssemblers();
	void Shutdown();

	void Update(float deltaTime);
	// @brief Updates systems.
	void UpdateRegistries(float deltaTime);
	// @brief Updates after systems have run, but before drawing.
	void LateUpdate(float deltaTime);
	void DrawGame(float deltaTime);

private:
	entt::registry registry { };
	RenderContext renderContext { };
	GameState gameState { };
	Settings settings { };
	Settings pendingSettings { };
	ResourceStore resourceStore { };

	void DrawRenderTexture() const;

#ifdef DEBUG_BUILD
	void DrawDebugUi() const;
#endif // DEBUG_BUILD

};