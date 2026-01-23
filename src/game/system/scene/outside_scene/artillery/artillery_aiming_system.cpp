#include "game/system/scene/outside_scene/artillery/artillery_aiming_system.hpp"

#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/contexts/system_context.hpp"


void System::Artillery::Aiming::Update(const SystemContext& context) noexcept
{
	const entt::entity entity = entt::get_single<Component::Artillery>(context.registry);
	auto& artillery = context.registry.get<Component::Artillery>(entity);

	if (artillery.isReadyToFire) return;
	if (artillery.aimStartupSeconds > 0.0f)
	{
		artillery.aimStartupSeconds -= context.deltaTime;
		return;
	}

	const Nc::Vector2f targetPosition = artillery.targetPosition;
	artillery.aimPosition = Nc::Vector::SmoothApproach(artillery.aimPosition, targetPosition, context.deltaTime, 0.4f);
	const float sqrDistance = Nc::Vector::SqrDistanceBetween(artillery.aimPosition, artillery.targetPosition);

	if (sqrDistance > 0.1f) return;;

	artillery.aimPosition = artillery.targetPosition;
	artillery.isReadyToFire = true;
}
