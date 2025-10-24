 #pragma once
#include "core/game_state.hpp"
#include "core/render_context.hpp"
#include "core/resource_store.hpp"
#include "core/settings.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp" 

#ifdef DEBUG_BUILD
#include "core/debug_context.hpp"
#endif // DEBUG_BUILD


class Game
{
public:
#ifdef DEBUG_BUILD
	static DebugContext debugContext;
#endif // DEBUG_BUILD

	Game();

	bool ShouldRun() const;
	
	void BuildMenuUI();
	void BuildRuntimeScenes();
	void SetupWindow() const;
	void SetupRenderContext();

#ifdef DEBUG_BUILD
	void SetupDebug(int args, char* argv[]);
#endif // DEBUG_BUILD

	void Shutdown();

	void Update(float deltaTime);
	// @brief Updates systems.
	void UpdateRegistries(float deltaTime);
	void DrawGame(float deltaTime);

	// @brief Handles player death.
	static void Death(entt::registry& registry, GameState& gameState);

private:
	entt::registry registry { };
	ResourceStore resourceStore { };
	RenderContext renderContext { };
	GameState gameState { };
	Settings settings { };
	Settings pendingSettings { };

	void DrawRenderTexture() const;

#ifdef DEBUG_BUILD
	void DrawDebugUi() const;
#endif // DEBUG_BUILD

};