#pragma once
#include <cstdint>


namespace Component
{
/**
 * @brief Represents the radar machine entity.
 * 
 * Does not hold rendering data.
 */
struct Radar final
{
	// The time it takes to recalibrate the radar machine in seconds.
	static constexpr float RECALIBRATION_TIME = 18.0f;
	static constexpr float STABLE_LEVEL = 70.0f;
	static constexpr float HEALTHY_LEVEL = 45.0f;
	static constexpr float UNSTABLE_LEVEL = 15.0f;


	// ------ Members ------
	
	float stability = 100.0f;
	// Remaining seconds of recalibration.
	float recalibrationTimeLeft = 0.0f;
    float screenGlitchSecondsLeft = 0.0f;
    float screenGlitchSeconds = 0.2f;
    float screenGlitchWaitSecondsLeft = 0.0f;
    float screenGlitchStrength = 0.0f;
	float nextGlitchSpawnSeconds = 0.0f;
	float lastGlitchTime = 0.0f;
	float breakdownCheckTimer = 0.0f;
	uint8_t glitchCount = 0u;
	bool isRecalibrating = false;
    bool screenGlitchReversed = false;


	// ------ Constructors ------

	constexpr Radar() noexcept = default;
};


/**
 * Component that represents the warning message for the radar.
 * 
 * Holds `alpha` value for the blinking animation.
 */
struct RadarErrorWarning final
{
	enum Tween
	{
		BlinkFade = 0,
	};


	// ------ Members ------

	float alpha = 1.0f;


	// ------ Constructors ------

	constexpr RadarErrorWarning() noexcept = default;
};

}