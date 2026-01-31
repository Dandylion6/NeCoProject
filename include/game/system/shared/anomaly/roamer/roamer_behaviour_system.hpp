#pragma once
#include "core/data/vector2.hpp"
#include "game/component/shared/anomaly/roamer/anomaly_roamer_component.hpp"
struct SystemContext;
struct AnomalyState;


namespace System::Anomaly::Roamer
{

class Behaviour final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, AnomalyState& anomalyState) noexcept;
	static Nc::Vector2f GetTargetPosition(RoamerTarget target) noexcept;

private:
	// ------ Functions ------
	static float GetPressureWeight(RoamerBehaviour behaviour) noexcept;

};

}

