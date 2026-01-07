#pragma once
#include <cstdint>


/**
 * @brief Defines the stage of anomaly development.
 * 
 * Used to introduce new systems, behaviours and logic at predetermined points in progression.
 * The intensity level increases at set game dates. The reason this is used instead of the
 * date is to keep logic seperate from the day.
 */
using IntensityLevel = uint8_t;


struct AnomalyState final
{

	/**
	 * @brief The minimum attraction percentage at first night.
	 */
	static constexpr float BASE_ATTRACTION = 5.0f;

	/**
	 * @brief Represents the anomaly's attention toward the player as an absolute threshold (0-100%).
	 * 
	 * This means that behaviour tied to attraction doesn't scale based on difficulty.
	 * Defines how reactive or aggressive the entity is, with higher values unlocking harsher behaviour.
	 * A constant measure of danger that does not change meaning across nights.
	 */
	float attractionPercentage = BASE_ATTRACTION;
	float nextRoamerSpawnSecondsLeft = 0.0f;

	/**
	 * @brief A scaling value influencing how the world respons to progression.
	 * 
	 * Provides deterministic difficulty growth for behaviour that lends itself to it,
	 * systems that intensify perminently, such as attraction sensitivity.
	 */
	float intensityFactor = 1.0f;

	/**
	 * @brief The sum of pressure based on the roamers active.
	 */
	float roamerPressureWeight = 0.0f;

	IntensityLevel intensityLevel = 1u;
	uint8_t totalRoamerCount = 0u;
	uint8_t roamerThreatCount = 0u;


	/**
	 * @brief Used to determine how much the player should be pressured
	 * by roamers dependant on the `IntensityLevel`. 
	 */
	inline float GetPressureTarget(IntensityLevel level) const
	{
		if (level <= 2u) return 1.5f;
		if (level <= 5u) return 2.5f;
		return 4.0f;
	}


	/**
	 * @brief Gets a predetermined max roamer count dependant on the 
	 * `IntensityLevel` given.
	 */
	inline uint8_t GetMaxRoamers(IntensityLevel level) const
	{
		if (level <= 4) return 5u;
		return 8u;
	}
};