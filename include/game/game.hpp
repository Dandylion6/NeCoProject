#pragma once
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/game_state.hpp"
#include "game/state/settings.hpp"
#include "state/anomaly_state.hpp"
#include "state/game_events.hpp"


class Game
{
public:
	// ------ Functions ------
	Game();

	[[nodiscard]] bool ShouldRun() const;

	void BuildMenuUI();
	void BuildRuntimeScenes();
    void SetupWindow();
	void SetupRenderContext();

#ifdef DEBUG_BUILD
	void SetupDebug(int args, char* argv[]);
#endif // DEBUG_BUILD

	void Save();
	void Load();

	void Shutdown();

	void Update(float deltaTime);
	void UpdateSystems(float deltaTime);
	void DrawGame(float deltaTime);
	void HandleEvents();

	static void Death(const SceneContext& context);

private:
	// ------ Members ------
	entt::registry registry{ };
	Nc::ResourceStore resourceStore{ };
	Nc::RenderContext renderContext;
	AnomalyState anomalyState{ };
	GameState gameState{ };
	Settings settings{ };
	Settings pendingSettings{ };
	GameEvents gameEvents{ };


	// ------ Functions ------

	void DrawRenderTexture() const;

#ifdef DEBUG_BUILD
	void DrawDebugUi();
#endif // DEBUG_BUILD

};
