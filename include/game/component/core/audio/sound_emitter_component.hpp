#pragma once
#include "raylib.h"
#include <utility>


namespace Component
{
	struct SoundEmitter
	{
		Sound sound;
		float volume = 1.0f;
		
		SoundEmitter(): sound() { };
		SoundEmitter(Sound&& sound): sound(std::move(sound)) { };
	};


	struct LoopedSoundEmitter
	{
		Music sound;
		float volume = 1.0f;

		LoopedSoundEmitter(): sound() { };
		LoopedSoundEmitter(Music&& sound): sound(std::move(sound)) { };
	};
}