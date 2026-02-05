#pragma once
#include <cassert>
#include <cstdint>

#include "cursor.hpp"
#include "core/data/bounds.hpp"
#include "core/data/vector2.hpp"
#include "game/state/scene.hpp"


/**
 * @brief The in-game representation of days since the start of the game.
 */
using Day = uint8_t;


constexpr auto BUNKER_POSITION = Nc::Vector2f(48.0f, -57.0f);
constexpr auto ARTILLERY_POSITION = Nc::Vector2f(-62.5f, 2.0f);
constexpr auto RADAR_POSITION = Nc::Vector2f(282.0f, 100.0f);
constexpr auto WORLD_BOUNDS = Nc::Bounds(-100.0f, 100.0f, 100.0f, -100.0f); // Y-axis is flipped.
constexpr auto RADAR_BOUNDS = Nc::Bounds(0.0f, 0.0f, 320.0f, 320.0f);


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
	 * @brief The clock hours when the 'night' gameplay occurs.
	 * 
	 * Represents a circular time frame on a 24-hour clock.
	 * The range wraps around past midnight.
	 */
	static constexpr auto NIGHT_RANGE = Nc::Vector2f(18.0f, 2.0f);

	/**
	 * @brief Current clock `hour` value, from `0.0f` to `23.999...`.
	 */
	float hour = 0.0f;
	float time = 0.0f;
	Day day = 0u;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	Nc::Cursor::Type cursor = Nc::Cursor::Standard;
	bool isPaused = false;

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
	 * This is what the night is agreed to be in-game, not in real life.
	 * 
	 * @param hour is the clock value hour from `0.0f` to `23.999...`. 
	 * @return whether the given `hour` lies inside the range.
	 */
	[[nodiscard]] static bool IsNight(const float hour)
	{
#ifdef DEBUG_BUILD
		assert(NIGHT_RANGE.y < NIGHT_RANGE.x && "Illogical night time range!");
#endif
		return hour >= NIGHT_RANGE.x || hour < NIGHT_RANGE.y;
	}
};
