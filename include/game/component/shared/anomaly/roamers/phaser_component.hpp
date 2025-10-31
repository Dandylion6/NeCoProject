#pragma once
#include "utility/vector2.hpp"
#include <array>
#include <cstdint>


namespace Component::Anomaly
{
	struct Phaser
	{
		static constexpr uint8_t MAX_POINT_COUNT = 3u;

		std::array<Nc::Vector2f, MAX_POINT_COUNT> points { };
		// todo: add support for variable point counts. For now, 2 points are used.
		uint8_t pointCount = 2u;
		uint8_t currentPointIndex = 0u;
		float secondsUntilNextPoint = 0.0f;
	};
}