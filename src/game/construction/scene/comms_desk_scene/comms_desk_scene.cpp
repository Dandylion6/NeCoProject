#include "game/construction/scene/comms_desk_scene/comms_desk_scene.hpp"

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/runtime/resource_store.hpp"
#include "entt/entity/fwd.hpp"
#include "game/construction/scene/comms_desk_scene/entity/journal_entity.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_ambience_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/contexts/build_context.hpp"
#include "game/contexts/scene_context.hpp"
#include "game/state/scene.hpp"


void Structure::DeskScene::Build(const BuildContext& context) noexcept
{
    constexpr char SCENE_AMBIENCE_PATH[] = "assets/audio/ambient/bunker_interior_ambience.wav";
    constexpr char SCENE_ALBEDO_PATH[] = "assets/environment/backgrounds/comms_desk/desk_scene_albedo.png";
    constexpr char SCENE_NORMAL_PATH[] = "assets/environment/backgrounds/comms_desk/desk_scene_normal.png";
    constexpr char SCENE_AO_PATH[] = "assets/environment/backgrounds/comms_desk/desk_scene_ao.png";

    constexpr auto LIGHT_COLOR = Nc::Hex(0xfff3c9ff);
    constexpr auto LIGHT_POSITION = Nc::Vector2f(520.0f, 310.0f);
    constexpr float LIGHT_RADIUS = 1580.0f;
    constexpr float MOVE_TIME = 0.14f;

    const SceneContext sceneContext = SceneContext(context.registry, context.store, context.game);

    Entity::Journal::Create(sceneContext);

    const Texture2D& sceneAlbedoTexture = context.store.GetTexture(SCENE_ALBEDO_PATH);
    const Texture2D& sceneNormalTexture = context.store.GetTexture(SCENE_NORMAL_PATH);
    const Texture2D& sceneAoTexture = context.store.GetTexture(SCENE_AO_PATH);

    Entity::SceneBackground::Create(
        context.registry,
        sceneAlbedoTexture,
        sceneNormalTexture,
        sceneAoTexture,
        CommsDesk
    );

    Entity::MoveRegion::Create(sceneContext, Up, CommsDesk, CommsRoom, MOVE_TIME);
    Entity::LightPoint::Create(
        context.registry,
        CommsDesk,
        LIGHT_POSITION,
        380.0f,
        Nc::RGBa(LIGHT_COLOR),
        1.36f,
        LIGHT_RADIUS
    );

    Entity::SceneAmbience::Create(sceneContext, SCENE_AMBIENCE_PATH, CommsDesk);
}
