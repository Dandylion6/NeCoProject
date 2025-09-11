#pragma once
#include "core/scene.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"
#include <cstdint>


struct AnomalyState
{
	static constexpr float DEGRADATION_THRESHOLD = 20.0f; // The attraction percentage needed to start degrading stability.
	static constexpr float PRECENTAGE_FACTOR = 1.0f / (100.0f - DEGRADATION_THRESHOLD);

	float lastSpawnTime = 0.0f;
	float spawnWaitMinutes = 0.1f;
	float attractionPercentage = 10.0f;
	uint8_t intensityLevel = 1u;
};


struct GameState
{
	static constexpr Nc::Vector2f BUNKER_POSITION = Nc::Vector2f(48.0f, 57.0f);
	static constexpr Nc::Vector2f ARTILLERY_POSITION = Nc::Vector2f(-62.5f, -2.0f);
	static constexpr Nc::Bounds WORLD_BOUNDS = Nc::Bounds(-100.0f, -100.0f, 100.0f, 100.0f);

	AnomalyState anomalyState { };
	float time = 0.0f;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
	bool shouldExit = false;
};