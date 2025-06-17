#pragma once
#include "utility/tween.h"
#include <array>
#include <cstdint>
#include <type_traits>


namespace Component
{
	struct TweenCollection
	{
		static constexpr uint8_t MAX_TWEENS = 4u;
		std::array<Tween, MAX_TWEENS> tweens { };
		
		TweenCollection() = default;
		TweenCollection(std::array<Tween, MAX_TWEENS>&& tweens): tweens(std::move(tweens)) { };
	};
}