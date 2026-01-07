#pragma once
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/scene.hpp"
#include <cstdint>

/**
 * @brief The in-game representation of days since the start of the game.
 */
using Day = uint8_t;


constexpr Nc::Vector2f BUNKER_POSITION = Nc::Vector2f(48.0f, -57.0f);
constexpr Nc::Vector2f ARTILLERY_POSITION = Nc::Vector2f(-62.5f, 2.0f);
constexpr Nc::Vector2f RADAR_POSITION = Nc::Vector2f(282.0f, 100.0f);
constexpr Nc::Bounds WORLD_BOUNDS = Nc::Bounds(-100.0f, 100.0f, 100.0f, -100.0f); // Y-axis is flipped.
constexpr Nc::Bounds RADAR_BOUNDS = Nc::Bounds(0.0f, 0.0f, 320.0f, 320.0f);


struct GameState final
{
	/**
	 * @brief How many minutes an in-game hour is.
	 */
	static constexpr float HOUR_MINUTES = 2.2f;

	/**
	 * @brief The hour the player wakes up at.
	 */
	static constexpr float WAKE_HOUR = 7.0f;

	/**
	 * @brief The clock hours where the 'night' gameplay occurs.
	 * 
	 * Represents a circular time frame on a 24-hour clock.
	 * The range wraps around past midnight.
	 */
	static constexpr Nc::Vector2f NIGHT_RANGE = Nc::Vector2f(18.0f, 1.0f);

	AnomalyState anomalyState { };
	float time = 0.0f;

	/**
	 * @brief Current clock `hour` value, from `0.0f` to `23.999...`.
	 */
	float hour = 0.0f;
	Day day = 0u;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
	bool shouldExit = false;

	/**
	 * @brief Indicates whether the player completed the night phase.
	 * 
	 * Value becomes `true` once the night is over.
	 * Goes back to being `false` once the player goes to sleep and the next day begins.
	 */
	bool survivedNight = false;

	
	/**
	 * @brief Checks whether the given `hour` is within the
	 * determined `NIGHT_RANGE`.
	 * 
	 * This is what the night is agrred to be in-game, not in real life.
	 * 
	 * @param hour is the clock value hour from `0.0f` to `23.999...`. 
	 * @return whether the given `hour` lies inside the range.
	 */
	inline bool IsNight(float hour) const
	{
		if (NIGHT_RANGE.x < NIGHT_RANGE.y)
			return hour >= NIGHT_RANGE.x && hour < NIGHT_RANGE.y + 1.0f;
		return hour >= NIGHT_RANGE.x || hour < NIGHT_RANGE.y + 1.0f;
	}
};