#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"


struct SystemContext;


namespace System::Radar
{

class Artillery final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context);

private:
	// ------ Functions ------
	static Nc::Vector2f GetArtilleryAimPosition(entt::registry& registry);

};

}

