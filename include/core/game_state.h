#pragma once
#include "core/scene.h"
#include <cstdint>


struct GameState
{
	static constexpr uint8_t FRAME_RATE = 120u;

	float time = 0.0f;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
};