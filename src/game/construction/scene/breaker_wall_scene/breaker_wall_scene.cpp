#include "game/construction/scene/breaker_wall_scene/breaker_wall_scene.hpp"

#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/render_context.hpp"
#include "core/runtime/resource_store.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/contexts/build_context.hpp"
#include "game/contexts/scene_context.hpp"


void Structure::BreakerWallScene::Build(const BuildContext& context) noexcept
{
    constexpr char SCENE_ALBEDO_PATH[] = "assets/environment/backgrounds/breaker_wall/breaker_scene_albedo.png";
    constexpr char SCENE_NORMAL_PATH[] = "assets/environment/backgrounds/breaker_wall/breaker_scene_normal.png";
    constexpr char SCENE_AO_PATH[] = "assets/environment/backgrounds/breaker_wall/breaker_scene_ao.png";

    constexpr auto LIGHT_COLOR = Nc::Hex(0xf3ffbfff);
    constexpr Nc::Vector2f LIGHT_POSITION = Nc::Vector::Modulate(
        Nc::Vector2f(0.58f, 1.25f),
        Nc::Vector2f(Nc::RENDER_RESOLUTION)
    );
    constexpr float LIGHT_DISTANCE = 350.0f;
    constexpr float LIGHT_RADIUS = 1620.0f;

    const SceneContext sceneContext = SceneContext(context.registry, context.store, context.game);

    const Texture2D& sceneAlbedoTexture = context.store.GetTexture(SCENE_ALBEDO_PATH);
    const Texture2D& sceneNormalTexture = context.store.GetTexture(SCENE_NORMAL_PATH);
    const Texture2D& sceneAoTexture = context.store.GetTexture(SCENE_AO_PATH);

    Entity::SceneBackground::Create(
        context.registry,
        sceneAlbedoTexture,
        sceneNormalTexture,
        sceneAoTexture,
        BreakerWall
    );

    Entity::MoveRegion::Create(sceneContext, Right, BreakerWall, CommsRoom);
    Entity::LightPoint::Create(
        context.registry,
        BreakerWall,
        LIGHT_POSITION,
        LIGHT_DISTANCE,
        Nc::RGBa(LIGHT_COLOR),
        1.38f,
        LIGHT_RADIUS
    );
}
