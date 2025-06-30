#pragma once
#include "core/scene.h"
#include "utility/morse_code.h"
#include <cstdint>


struct MorseSettings
{
	float dotTime = 0.16f;
	float dashTime = dotTime * 3.0f;
	float errorMargin = dotTime * MorseCode::ERROR_MARGIN;
	float exitTime = dashTime + errorMargin + dotTime;
};


struct GameState
{
	static constexpr uint8_t FRAME_RATE = 120u;

	MorseSettings morseSettings { };
	float time = 0.0f;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
};