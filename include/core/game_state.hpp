#pragma once
#include "core/scene.hpp"
#include "utility/bounds.hpp"
#include "utility/vector2.hpp"
#include <cstdint>


using IntensityLevel = uint8_t;


struct AnomalyState
{
	// @brief The minimum attraction percentage at first night.
	static constexpr float BASE_ATTRACTION = 5.0f;

	/// <summary>
	/// Represents the anomaly’s attention toward the player as an absolute threshold (0–100%).
	/// Defines how reactive or aggressive the entity becomes, with higher values unlocking harsher behaviour.
	/// A constant measure of danger that does not change meaning across nights.
	/// </summary>
	float attractionPercentage = BASE_ATTRACTION;
	float nextSpawnSecondsLeft = 0.0f;
	
	/// <summary>
	/// A dynamic scaling value influencing how the world responds to progression.
	/// Provides deterministic difficulty growth beyond simply counting nights,
	/// affecting systems that evolve or intensify over time, such as attraction sensitivity.
	/// </summary>
	float intensityFactor = 1.0f;

	// @brief The sum of pressure based on the roamers active.
	float roamerPressureWeight = 0.0f;
	
	/// <summary>
	/// Defines the current stage of anomaly development.
	/// Used to introduce new systems, behaviours, and logic at predetermined points in progression.
	/// Represents qualitative growth rather than numeric escalation.
	/// </summary>
	IntensityLevel intensityLevel = 1u;
	uint8_t totalRoamerCount = 0u;
	uint8_t roamerThreatCount = 0u;


	static float GetPressureTarget(IntensityLevel level)
	{
		if (level <= 2u) return 1.5f;
		if (level <= 5u) return 2.5f;
		return 4.0f;
	}

	static uint8_t GetMaxRoamers(IntensityLevel level)
	{
		if (level <= 4) return 5u;
		return 8u;
	}
};


struct GameState
{
	static constexpr Nc::Vector2f BUNKER_POSITION = Nc::Vector2f(48.0f, -57.0f);
	static constexpr Nc::Vector2f ARTILLERY_POSITION = Nc::Vector2f(-62.5f, 2.0f);
	static constexpr Nc::Bounds WORLD_BOUNDS = Nc::Bounds(-100.0f, 100.0f, 100.0f, -100.0f); // Y-axis is flipped.

	AnomalyState anomalyState { };
	float time = 0.0f;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
	bool shouldExit = false;
};