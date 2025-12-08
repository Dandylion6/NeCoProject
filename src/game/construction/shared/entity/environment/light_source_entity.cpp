#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/component/core/rendering/lighting/light_source_component.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/state/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"


const entt::entity Construct::LightPointEntity(
    entt::registry& registry, Nc::Vector2f position, Scene boundScene, Nc::RGBa color, float strength, float range
)
{
    const entt::entity entity = registry.create();

    registry.emplace<Component::Transform>(entity, boundScene, position);
    registry.emplace<Component::Light::Point>(entity, color, range, strength);

    return entity;
}