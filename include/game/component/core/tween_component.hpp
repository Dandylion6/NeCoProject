#pragma once
#include "core/data/tween.hpp"
#include <array>
#include <cstdint>


namespace Component
{
	struct TweenCollection
	{
		static constexpr uint8_t MAX_TWEENS = 4u;
		std::array<Tween, MAX_TWEENS> tweens { };
		
		TweenCollection() = default;
		TweenCollection(std::array<Tween, MAX_TWEENS>&& tweens): 
			tweens(std::move(tweens)) { };
	};
}