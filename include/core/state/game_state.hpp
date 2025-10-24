#pragma once
#include "core/scene.hpp"
#include "core/state/anomaly_state.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"
#include <cstdint>


using Day = uint8_t;

constexpr Nc::Vector2f BUNKER_POSITION = Nc::Vector2f(48.0f, -57.0f);
constexpr Nc::Vector2f ARTILLERY_POSITION = Nc::Vector2f(-62.5f, 2.0f);
constexpr Nc::Vector2f RADAR_POSITION = Nc::Vector2f(282.0f, 100.0f);
constexpr Nc::Bounds WORLD_BOUNDS = Nc::Bounds(-100.0f, 100.0f, 100.0f, -100.0f); // Y-axis is flipped.
constexpr Nc::Bounds RADAR_BOUNDS = Nc::Bounds(0.0f, 0.0f, 320.0f, 320.0f);


struct GameState
{
	// @brief How many minutes an in-game hour is.
	static constexpr float HOUR_MINUTES = 2.2f;
	static constexpr float WAKE_HOUR = 7.0f;
	// @brief The clock hours where the 'night' gameplay occurs.
	static constexpr Nc::Vector2f NIGHT_RANGE = Nc::Vector2f(18.0f, 1.0f);

	AnomalyState anomalyState { };
	float time = 0.0f;
	float hour = 0.0f;
	Day day = 0u;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
	bool shouldExit = false;
	bool survivedNight = false;

	
	static bool IsNight(float hour)
	{
		if (NIGHT_RANGE.x < NIGHT_RANGE.y)
			return hour >= NIGHT_RANGE.x && hour < NIGHT_RANGE.y + 1.0f;
		return hour >= NIGHT_RANGE.x || hour < NIGHT_RANGE.y + 1.0f;
	}
};