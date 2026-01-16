#pragma once
#include <cstdint>


/**
 * @brief Defines the stage of anomaly development.
 * 
 * Used to introduce new systems, behaviors and logic at predetermined points in progression.
 * The intensity level increases at set game dates. The reason this is used instead of the
 * date is to keep logic separate from the day.
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
	 * This means that behavior tied to attraction doesn't scale based on difficulty.
	 * Defines how reactive or aggressive the entity is, with higher values unlocking harsher behavior.
	 * A constant measure of danger that does not change meaning across nights.
	 */
	float attractionPercentage = BASE_ATTRACTION;
	float nextRoamerSpawnSecondsLeft = 0.0f;

	/**
	 * @brief A scaling value influencing how the world response to progression.
	 * 
	 * Provides deterministic difficulty growth for behavior that lends itself to it,
	 * systems that intensify permanently, such as attraction sensitivity.
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
	 * by roamers dependent on the <c>IntensityLevel</c>.
	 */
	[[nodiscard]] static float GetPressureTarget(const IntensityLevel level) noexcept
	{
		if (level <= 2u) return 1.5f;
		if (level <= 5u) return 2.5f;
		return 4.0f;
	}


	/**
	 * @brief Gets a predetermined max roamer count dependent on the
	 * <c>IntensityLevel</c> given.
	 */
	[[nodiscard]] static uint8_t GetMaxRoamers(const IntensityLevel level) noexcept
	{
		if (level <= 4) return 5u;
		return 8u;
	}
};