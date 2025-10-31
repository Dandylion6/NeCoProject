#pragma once
#include <cstdint>


namespace Component
{
	struct Radar
	{
		// @brief The time it takes to recalibrate the radar machine in seconds.
		static constexpr float RECALIBRATION_TIME = 16.0f;
		static constexpr float STABLE_LEVEL = 70.0f;
		static constexpr float HEALTHY_LEVEL = 40.0f;
		static constexpr float UNSTABLE_LEVEL = 15.0f;

		float stability = 100.0f;
		// Remaining seconds of recalibration.
		float recalibrationTimeLeft = 0.0f;
		float nextGlitchSpawnSeconds = 0.0f;
		float lastGlitchTime = 0.0f;
		float breakdownCheckTimer = 0.0f;
		uint8_t glitchCount = 0u;
		bool isRecalibrating = false;
	};
}


namespace Component
{
	struct RadarErrorWarning
	{
		enum Tween 
		{
			BlinkFade = 0,
		};

		float alpha = 1.0f;
	};
}