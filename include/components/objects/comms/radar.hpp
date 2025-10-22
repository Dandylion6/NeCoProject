#pragma once
#include <cstdint>
#include <array>


namespace Component
{
	struct Radar
	{
		// @brief The time it takes to recalibrate the radar machine in seconds.
		static constexpr float RECALIBRATION_TIME = 12.0f;
		static constexpr float STABLE_LEVEL = 80.0f;
		static constexpr float HEALTHY_LEVEL = 50.0f;
		static constexpr float UNSTABLE_LEVEL = 20.0f;

		// @brief The attraction percentage needed to start degrading stability.
		static constexpr float DEGRADATION_THRESHOLD = 10.0f;
		static constexpr float PERCENTAGE_FACTOR = 1.0f / (100.0f - DEGRADATION_THRESHOLD);

		static constexpr uint8_t CURVE_CACHE_SIZE = 10u;
		std::array<float, CURVE_CACHE_SIZE> degredationCurveCache { };

		float stability = 100.0f;
		// Remaining seconds of recalibration. If the value is above zero then the machine is recalibrating.
		float recalibrationTimeLeft = -1.5f;
		float nextGlitchSpawnSeconds = 0.0f;
		float lastGlitchTime = 0.0f;
		uint8_t glitchCount = 0u;
	};

	struct RadarErrorWarning
	{
		enum Tween 
		{
			BlinkFade = 0,
		};

		float alpha = 1.0f;
	};
}


namespace Tag
{
	struct RadarPath { };

	struct RadarArtillery { };

	struct RadarRecalibration { };

	struct RadarButton { };
}