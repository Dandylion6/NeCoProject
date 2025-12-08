#pragma once
#include "core/math/interpolation.hpp"
#include <cstdint>
#include <functional>


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


struct Tween final
{
	// ------ Members ------

	// TODO: Use entt delegates instead.
	std::function<void()> onComplete { };
	float* value = nullptr;
	float start = 0.0f;
	float end = 1.0f;
	float duration = 1.0f;
	float elapsed = 0.0f;
	float delayComplete = 0.0f;
	Easing easing = Linear;
	bool isPlaying = false;


	// ------ Constructors ------

	constexpr Tween() noexcept = default;
	Tween(
		float* value, 
		float start, 
		float end, 
		float duration, 
		Easing easing, 
		float delayComplete = 0.0f
	) noexcept :
		value(value),
		start(start),
		end(end),
		duration(duration),
		easing(easing),
		delayComplete(delayComplete)
	{ };

	static constexpr void Build(
		Tween& tween,
		float* value,
		float start,
		float end,
		float duration,
		Easing easing,
		float delayComplete = 0.0f
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

	static constexpr float GetEasing(
		Easing easing, float value
	) noexcept
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

	static constexpr void Play(Tween& tween) noexcept { tween.isPlaying = true; };
	static constexpr void Stop(Tween& tween) noexcept { tween.isPlaying = false; };
	static constexpr void Replay(Tween& tween) noexcept
	{
		tween.elapsed = 0.0f;
		Tween::Play(tween);
	}
};