#include "game/save/save_game.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "game/component/core/interactive/toggle_component.hpp"
#include "game/component/core/serialization/address_component.hpp"
#include "game/component/scene/comms_scene/radar_components.hpp"
#include "game/component/shared/mechanical/lever_component.hpp"
#include "game/contexts/states_context.hpp"
#include "game/state/anomaly_state.hpp"
#include "game/state/game_state.hpp"
#include "game/state/scene.hpp"
#include "game/system/shared/mechanical/lever_system.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"


#ifdef RELEASE_BUILD
#include "base64.hpp"
#endif // RELEASE_BUILD

// TODO: Error handling for file I/O and JSON parsing.


namespace Save
{

static Result SaveState(const StatesContext& context, nlohmann::json& data)
{
    nlohmann::json& gameStateData = data["game_state"];

    // Game state saving
    gameStateData["day"] = context.game.day;
    gameStateData["time"] = context.game.time;
    gameStateData["current_scene"] = context.game.currentScene;

    // Anomaly state saving
    gameStateData["attraction_percentage"] = context.anomaly.attractionPercentage;

    return Result::Success;
}


static Result SaveToggleComponents(entt::registry& registry, nlohmann::json& data)
{
    const auto view = registry.view<const Component::Address, const Component::Action::Toggle>();
    for (auto [entity, address, toggle] : view.each())
    {
        if (address.address.empty()) return Result::EmptyAddressComponent;

        nlohmann::json& entityData = data["entities"][address.address];

        entityData["toggle_state"] = toggle.state;

        // In the case where the machine has other relevant components
        if (registry.any_of<Component::Radar>(entity))
        {
            const auto& radar = registry.get<Component::Radar>(entity);
            entityData["radar_stability"] = radar.stability;
            entityData["radar_recalibration_time"] = radar.recalibrationTimeLeft;
        }
    }
    return Result::Success;
}


Result GameToDisk(entt::registry& registry, const StatesContext& context)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);

    std::ofstream stream(dataDirectoryPath / "game.save", std::ios::out);
    nlohmann::json data;

    SaveState(context, data);
    SaveToggleComponents(registry, data);

#ifdef DEBUG_BUILD
    stream << data.dump(4) << std::endl;
#else
    std::string dataString = data.dump();
    std::string encodedData = base64::to_base64(dataString);
    stream << encodedData;
#endif

    stream.close();
    return Result::Success;
}

}


namespace Load
{

static Result LoadGameState(const StatesContext& context, nlohmann::json& data)
{
    if (!data.contains("game_state"))
    {
        // Load starting data
        nlohmann::json& gameStateData = data["game_state"];
        gameStateData["attraction_percentage"] = AnomalyState::BASE_ATTRACTION;
        gameStateData["day"] = 0;
        gameStateData["time"] = 0.0f;
        gameStateData["current_scene"] = static_cast<uint8_t>(CommsRoom);
    }

    nlohmann::json& gameStateData = data["game_state"];

    // Load game state
    context.game.day = gameStateData["day"];
    context.game.hour = GameState::WAKE_HOUR;
    context.game.time = gameStateData["time"];
    context.game.currentScene = static_cast<Scene>(gameStateData["current_scene"]);

    // Load anomaly state
    context.anomaly.attractionPercentage = gameStateData["attraction_percentage"];

    return Result::Success;
}


static Result LoadToggleComponents(entt::registry& registry, nlohmann::json& data)
{
    nlohmann::json& entitiesData = data["entities"];
    const auto view = registry.view<Component::Address, Component::Action::Toggle>();
    for (auto [entity, address, toggle] : view.each())
    {
        if (!entitiesData.contains(address.address)) return Result::AddressComponentNotFound;

        nlohmann::json& entityData = entitiesData[address.address];
        if (!entityData.contains("machine_is_active")) return Result::ComponentValueNotFound;
        toggle.state = entityData["toggle_state"];

        if (registry.any_of<Component::Radar>(entity))
        {
            if (!entityData.contains("radar_stability")) return Result::ComponentValueNotFound;
            if (!entityData.contains("radar_recalibration_time")) return Result::ComponentValueNotFound;

            Component::Radar& radar = registry.get<Component::Radar>(entity);
            radar.stability = entityData["radar_stability"];
            radar.recalibrationTimeLeft = entityData["radar_recalibration_time"];
        }

        // Changes lever to reflect its toggle state
        if (registry.any_of<Component::Logic::Lever>(entity))
        {
            Component::Logic::Lever& lever = registry.get<Component::Logic::Lever>(entity);
            lever.currentHeight = LeverSystem::GetHeightTarget(toggle.state, lever);
        }
    }
    return Result::Success;
}


Result GameFromDisk(entt::registry& registry, const StatesContext& context)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);

    std::ifstream stream(dataDirectoryPath / "game.save", std::ios::in);

#ifdef DEBUG_BUILD
    nlohmann::json data{ };
    if (stream.is_open()) data = nlohmann::json::parse(stream);
    stream.close();
#else
    std::stringstream stringBuffer;
    stringBuffer << stream.rdbuf();

    std::string decodedData = base64::from_base64(stringBuffer.view());
    nlohmann::json data;
    if (!decodedData.empty()) data = nlohmann::json::parse(decodedData);
#endif

    LoadGameState(context, data);
    LoadToggleComponents(registry, data);

    return Result::Success;
}


}
