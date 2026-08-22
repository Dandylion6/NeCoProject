#include "game/construction/shared/entity/environment/light_source_entity.hpp"

#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/transform_component.hpp"
#include "game/component/core/rendering/lighting/point_light_component.hpp"
#include "game/state/scene.hpp"


entt::entity Entity::LightPoint::Create(
    entt::registry& registry, 
    Scene boundScene, 
    Nc::Vector2f position,
    const float zPosition,
    Nc::RGBa color, 
    float strength, 
    float range,
    float flickerStrength
) noexcept
{
    const entt::entity entity = registry.create();

    registry.emplace<Component::Transform>(entity, boundScene, position);
    registry.emplace<Component::Light::Point>(entity, color, zPosition, range, strength, flickerStrength);

    return entity;
}
