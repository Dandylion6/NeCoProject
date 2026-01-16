#pragma once
#include "entt/entity/fwd.hpp"


namespace System::Blip
{

class Death final
{
public:
	// ------ Functions ------
	static void Update(entt::registry& registry);

};

}