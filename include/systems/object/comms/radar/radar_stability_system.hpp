#pragma once
struct AnomalyState;


class RadarStabilitySystem
{
public:
	static void Update(AnomalyState& anomalyState, float deltaTime);
};