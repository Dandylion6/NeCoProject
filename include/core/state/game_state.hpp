#pragma once
#include "core/scene.hpp"
#include "core/state/anomaly_state.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"


constexpr Nc::Vector2f BUNKER_POSITION = Nc::Vector2f(48.0f, -57.0f);
constexpr Nc::Vector2f ARTILLERY_POSITION = Nc::Vector2f(-62.5f, 2.0f);
constexpr Nc::Vector2f RADAR_POSITION = Nc::Vector2f(282.0f, 100.0f);
constexpr Nc::Bounds WORLD_BOUNDS = Nc::Bounds(-100.0f, 100.0f, 100.0f, -100.0f); // Y-axis is flipped.
constexpr Nc::Bounds RADAR_BOUNDS = Nc::Bounds(0.0f, 0.0f, 320.0f, 320.0f);


struct GameState
{
	AnomalyState anomalyState { };
	float time = 0.0f;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
	bool shouldExit = false;
};