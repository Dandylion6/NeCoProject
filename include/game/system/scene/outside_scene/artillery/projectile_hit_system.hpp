#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct SystemContext;


namespace System::Projectile
{

class Hit final
{
public:
	// ------ Members ------
	static constexpr size_t MAX_HITS = 4u;


	// ------ Functions ------

	static void Update(const SystemContext& context) noexcept;

private:
	// ------ Functions ------
	static void CheckForHits(entt::registry& registry, Nc::Vector2f hitPositions[MAX_HITS], size_t hitCount) noexcept;

};

}
