 #pragma once
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp" 
#include "game/state/game_state.hpp"
#include "game/state/settings.hpp"

#ifdef DEBUG_BUILD
#include "game/debug/debug_context.hpp"
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
	Nc::ResourceStore resourceStore { };
	Nc::RenderContext renderContext { };
	GameState gameState { };
	Settings settings { };
	Settings pendingSettings { };

	void DrawRenderTexture() const;

#ifdef DEBUG_BUILD
	void DrawDebugUi() const;
#endif // DEBUG_BUILD

};
