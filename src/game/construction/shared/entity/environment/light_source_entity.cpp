#include "assemblers/entities/light_source_entity.hpp"
#include "components/core/lights/light_source_component.hpp"
#include "components/core/transform_component.hpp"
#include "core/scene.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "utility/color.hpp"
#include "utility/vector2.hpp"


const entt::entity Construct::LightSourceEntity(
    entt::registry& registry, Nc::Vector2f position, Scene boundScene, Nc::RGBa color, float strength, float range
)
{
    const entt::entity entity = registry.create();

    registry.emplace<Component::Transform>(entity, boundScene, position);
    registry.emplace<Component::LightSource>(entity, color, range, strength);

    return entity;
}