#pragma once
#include <cstdint>

#include "core/math/interpolation.hpp"


enum Easing : uint16_t
{
	Linear,
	SineInOut,
	CubicOut,
	BackOut,
	QuadIn,
	QuadOut,
	ExpoIn,
	ExpoOut,
};


namespace Nc
{
struct Tween final
{
	// ------ Types ------
	enum State : uint8_t
	{
		Stopped, Playing, Completed
	};


	// ------ Members ------

	float* value = nullptr;
	float start = 0.0f;
	float end = 1.0f;

	/**
	 * The amount of time the animation runs for in <c>seconds</c>.
	 */
	float duration = 1.0f;
	float elapsed = 0.0f;

	/**
	 * The amount of <c>seconds</c> to wait after the tween is finished before
	 * making it considered complete.
	 */
	float delayComplete = 0.0f;
	Easing easing = Linear;
	State state = Stopped;
	bool justCompleted = false;


	// ------ Constructors ------

	constexpr Tween() noexcept = default;
	Tween(
		float* value,
		const float start,
		const float end,
		const float duration,
		const Easing easing,
		const float delayComplete = 0.0f
	) noexcept :
		value(value),
		start(start),
		end(end),
		duration(duration),
		delayComplete(delayComplete),
		easing(easing) { }

	static constexpr void Build(
		Tween& tween,
		float* value,
		const float start,
		const float end,
		const float duration,
		const Easing easing,
		const float delayComplete = 0.0f
	) noexcept
	{
		tween.value = value;
		tween.start = start;
		tween.end = end;
		tween.duration = duration;
		tween.easing = easing;
		tween.delayComplete = delayComplete;
	}


	// ------ Utility ------

	static constexpr float GetEasing(const Easing easing, const float value) noexcept
	{
		switch (easing)
		{
		case Linear: return value;
		case SineInOut: return Nc::Math::SineInOut(value);
		case QuadIn: return Nc::Math::QuadIn(value);
		case QuadOut: return Nc::Math::QuadOut(value);
		case CubicOut: return Nc::Math::CubicOut(value);
		case ExpoIn: return Nc::Math::ExpoIn(value);
		case ExpoOut: return Nc::Math::ExpoOut(value);
		case BackOut: return Nc::Math::BackOut(value);
		default: return value;
		}
	}

	
	static constexpr void Play(Tween& tween) noexcept { tween.state = Playing; }
	static constexpr void Stop(Tween& tween) noexcept { tween.state = Stopped; }
	static constexpr void Replay(Tween& tween) noexcept
	{
		tween.elapsed = 0.0f;
		Play(tween);
	}
};

}