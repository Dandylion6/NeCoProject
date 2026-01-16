#pragma once
#include "entt/entity/entity.hpp"
struct SystemContext;
struct AnomalyState;


namespace System::Machine
{

class PowerUsage final
{
public:
	// ------ Functions -------
	static void Update(const SystemContext& context, AnomalyState& anomaly);

private:
	static bool IsMachineActive(const SystemContext& context, entt::entity machine) noexcept;

};

}

