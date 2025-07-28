#pragma once
#include "core/scene.hpp"
#include "utility/vector2.hpp"


struct AnomalyState
{
	float lastSpawnTime = 0.0f;
	float spawnWaitMinutes = 0.1f;
};


struct GameState
{
	static constexpr Nc::Vector2f BUNKER_POSITION = Nc::Vector2f(238.0f, 251.0f);
	static constexpr Nc::Vector2f ARTILLERY_POSITION = Nc::Vector2f(60.0f, 156.0f);

	AnomalyState anomalyState { };
	float time = 0.0f;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
	bool shouldExit = false;
};