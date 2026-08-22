#include "game/construction/scene/comms_scene/comms_scene.hpp"

#include "raylib.h"
#include "core/data/color.hpp"
#include "core/data/vector2.hpp"
#include "core/math/vector_math.hpp"
#include "core/runtime/render_context.hpp"
#include "game/construction/scene/comms_desk_scene/entity/morse_transceiver_entity.hpp"
#include "game/construction/scene/comms_desk_scene/object/morse_monitor_object.hpp"
#include "game/construction/scene/comms_scene/entity/radio_entity.hpp"
#include "game/construction/scene/comms_scene/object/radar_object.hpp"
#include "game/construction/shared/entity/environment/light_source_entity.hpp"
#include "game/construction/shared/entity/scene/move_region_entity.hpp"
#include "game/construction/shared/entity/scene/scene_ambience_entity.hpp"
#include "game/construction/shared/entity/scene/scene_background_entity.hpp"
#include "game/state/scene.hpp"


void Structure::CommsScene::Build(const BuildContext& context) noexcept
{
    constexpr char SCENE_AMBIENCE_PATH[] = "assets/audio/ambient/comms_ambience.wav";
    constexpr char SCENE_ALBEDO_PATH[] = "assets/environment/backgrounds/comms_room/comms_scene_albedo.png";
    constexpr char SCENE_NORMAL_PATH[] = "assets/environment/backgrounds/comms_room/comms_scene_normal.png";
    constexpr char SCENE_AO_PATH[] = "assets/environment/backgrounds/comms_room/comms_scene_ao.png";

    constexpr auto LIGHT_COLOR = Nc::Hex(0xfff3c9ff);
    constexpr Nc::Vector2f LIGHT_POSITION = Nc::Vector::Modulate(
        Nc::Vector2f(0.6f, 1.4f),
        Nc::Vector2f(Nc::RENDER_RESOLUTION)
    );
    constexpr float LIGHT_DISTANCE = 280.0f;
    constexpr float LIGHT_RADIUS = 1440.0f;

    const auto sceneContext = SceneContext(context.registry, context.store, context.game);

    Object::Radar::Create(sceneContext);
    Object::MorseMonitor::Create(sceneContext);
    Entity::MorseTransceiver::Create(sceneContext);
    Entity::Radio::Create(sceneContext);

    const Texture2D& sceneAlbedoTexture = LoadTexture(SCENE_ALBEDO_PATH);
    const Texture2D& sceneNormalsTexture = LoadTexture(SCENE_NORMAL_PATH);
    const Texture2D& sceneAoTexture = LoadTexture(SCENE_AO_PATH);

    Entity::SceneBackground::Create(
        context.registry,
        sceneAlbedoTexture,
        sceneNormalsTexture,
        sceneAoTexture,
        CommsRoom
    );

    Entity::MoveRegion::Create(sceneContext, Down, CommsRoom, CommsDesk, 0.15f);
    Entity::MoveRegion::Create(sceneContext, Right, CommsRoom, Doorway, 0.35f);
    Entity::LightPoint::Create(
        context.registry,
        CommsRoom,
        LIGHT_POSITION,
        LIGHT_DISTANCE,
        Nc::RGBa(LIGHT_COLOR),
        1.9f,
        LIGHT_RADIUS
    );

    Entity::SceneAmbience::Create(sceneContext, SCENE_AMBIENCE_PATH, CommsRoom);
}
