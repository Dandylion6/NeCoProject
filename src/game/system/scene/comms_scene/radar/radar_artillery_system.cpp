#include "game/system/scene/comms_scene/radar/radar_artillery_system.hpp"

#include "core/data/vector2.hpp"
#include "core/runtime/entity_helpers.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/scene/outside_scene/artillery_component.hpp"
#include "game/contexts/system_context.hpp"
#include "game/tag/scene/comms_scene/radar_tags.hpp"


void System::Radar::Artillery::Update(const SystemContext& context)
{
	const auto view = context.registry.view<const Tag::Radar::Artillery, Component::Transform>();
	for (auto [entity, transform] : view.each())
	{
		constexpr float ROTATION_SPEED = 60.0f;

		transform.position = GetArtilleryAimPosition(context.registry);
		transform.rotation += ROTATION_SPEED * context.deltaTime;
	}
};


Nc::Vector2f System::Radar::Artillery::GetArtilleryAimPosition(entt::registry& registry)
{
	const entt::entity entity = entt::get_single<Component::Artillery>(registry);
	const Component::Artillery& artillery = registry.get<Component::Artillery>(entity);
	return artillery.aimPosition;
}