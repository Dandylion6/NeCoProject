#pragma once
#include "core/scene.h"
#include "utility/morse_code.h"
#include "utility/vector2.h"


struct MorseSettings
{
	float dotTime = 0.16f;
	float dashTime = dotTime * 3.0f;
	float errorMargin = dotTime * MorseCode::ERROR_MARGIN;
	float exitTime = dashTime + errorMargin + dotTime;
};


struct AnomalyState
{
	float lastSpawnTime = 0.0f;
	float spawnWaitMinutes = 0.1f;
};


struct GameState
{
	static constexpr Nc::Vector2f BUNKER_POSITION = Nc::Vector2f(238.0f, 251.0f);
	static constexpr Nc::Vector2f ARTILLERY_POSITION = Nc::Vector2f(60.0f, 156.0f);

	MorseSettings morseSettings { };
	AnomalyState anomalyState { };
	float time = 0.0f;
	Scene currentScene = NullScene;
	Scene movingToScene = NullScene;
	bool isPaused = false;
};