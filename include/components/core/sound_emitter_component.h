#pragma once
#include "raylib.h"
#include <utility>


namespace Component
{
	struct SoundEmitter
	{
		Sound sound;
		float volume = 1.0f;
		float duration = 0.0f;
		float elapsed = 0.0f;
		bool isPlaying = false;
		bool loops = false;
		
		SoundEmitter() = default;
		SoundEmitter(
			Sound&& sound,
			bool loops = false
		):
			sound(std::move(sound)),
			loops(loops)
		{ };
	};
}