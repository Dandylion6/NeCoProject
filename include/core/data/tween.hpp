#pragma once
#include <cstdint>
#include <functional>


enum Easing: uint16_t
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


struct Tween
{

	std::function<void()> onComplete { };
	float* value = nullptr;
	float start = 0.0f;
	float end = 1.0f;
	float duration = 1.0f;
	float elapsed = 0.0f;
	float delayComplete = 0.0f;
	Easing easing = Linear;
	bool isPlaying = false;

	Tween() = default;
	Tween(
		float* value, 
		float start, 
		float end, 
		float duration, 
		Easing easing, 
		float delayComplete = 0.0f
	)
	{
		Build(value, start, end, duration, easing, delayComplete);
	};

	static float GetEasing(Easing easing, float value);
	static void Play(Tween& tween);
	static void Replay(Tween& tween);

	void Build(
		float* value,
		float start = 0.0f,
		float end = 1.0f,
		float duration = 1.0f,
		Easing easing = Linear,
		float delayComplete = 0.0f
	);
};