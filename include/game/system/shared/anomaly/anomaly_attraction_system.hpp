#pragma once
struct AnomalyState;
struct SystemContext;


namespace System::Anomaly
{

/**
 * @brief Handles attraction cooldown over time (only at night).
 *
 * @note The decay time should stay the same, only scaling the attraction gain to raise difficulty.
 */
class Attraction final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context, AnomalyState& anomaly);

};

}