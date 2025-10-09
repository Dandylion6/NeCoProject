#include "components/objects/comms/radar.hpp"
#include "components/objects/machine.hpp"
#include "components/scene/dont_destroy_on_load_tag.hpp"
#include "core/game.hpp"
#include "core/game_state.hpp"
#include "core/save_game.hpp"
#include "core/scene.hpp"
#include "core/settings.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


namespace Save
{


    static bool SaveGameState(GameState& gameState, std::ofstream& stream)
    {
        stream.write(reinterpret_cast<char*>(&gameState.anomalyState.attractionPercentage), sizeof(float));
        stream.write(reinterpret_cast<char*>(&gameState.time), sizeof(float));
        uint8_t rawScene = gameState.currentScene;
        stream.write(reinterpret_cast<char*>(&rawScene), sizeof(rawScene));
        return true;
    }


    static bool LoadGameState(GameState& gameState, std::ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&gameState.anomalyState.attractionPercentage), sizeof(float));
        stream.read(reinterpret_cast<char*>(&gameState.time), sizeof(float));
        uint8_t rawScene = static_cast<uint8_t>(CommsRoom);
        stream.read(reinterpret_cast<char*>(&rawScene), sizeof(rawScene));
        gameState.currentScene = static_cast<Scene>(rawScene);
        return true;
    }


    static bool SaveMachineComponents(entt::registry& registry, std::ofstream& stream)
    {
        auto view = registry.view<Component::Machine>();
        for (auto [entity, machine] : view.each())
        {
            stream.write(reinterpret_cast<char*>(&machine.isActive), sizeof(bool));

            // In the case where the machine has other relevant components
            if (registry.any_of<Component::RadarMachine>(entity))
            {
                Component::RadarMachine& radar = registry.get<Component::RadarMachine>(entity);
                stream.write(reinterpret_cast<char*>(&radar.stability), sizeof(float));
                stream.write(reinterpret_cast<char*>(&radar.recalibrationTimeLeft), sizeof(float));
            }
        }
        return true;
    }


    static bool LoadMachineComponents(entt::registry& registry, std::ifstream& stream)
    {
        auto view = registry.view<Component::Machine>();
        for (auto [entity, machine] : view.each())
        {
            stream.read(reinterpret_cast<char*>(&machine.isActive), sizeof(bool));

            // In the case where the machine has other relevant components
            if (registry.any_of<Component::RadarMachine>(entity))
            {
                Component::RadarMachine& radar = registry.get<Component::RadarMachine>(entity);
                stream.read(reinterpret_cast<char*>(&radar.stability), sizeof(float));
                stream.read(reinterpret_cast<char*>(&radar.recalibrationTimeLeft), sizeof(float));
            }
        }
        return true;
    }
}


bool Save::SaveGame(entt::registry& registry, GameState& gameState)
{
    if (gameState.save.empty()) return false; // No specified save.

    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ofstream stream(dataDirectoryPath / (gameState.save + ".save"), std::ios::out);

    //nlohmann::json data;

    Save::SaveGameState(gameState, stream);
    Save::SaveMachineComponents(registry, stream);

    stream.close();
    return true;
}


bool Save::LoadGame(Game& game, entt::registry& registry, GameState& gameState)
{
    if (gameState.save.empty()) return false; // No save specified to load from.

    // Cleans up the game before rebuilding
    for (const entt::entity entity : registry.view<const entt::entity>())
    {
        if (registry.any_of<Tag::DontDestroyOnLoad>(entity)) continue; // Ignores the entity
        registry.destroy(entity);
    }
    game.BuildRuntimeScenes();

    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ifstream stream(dataDirectoryPath / (gameState.save + ".save"), std::ios::in);

    Save::LoadGameState(gameState, stream);
    Save::LoadMachineComponents(registry, stream);

    stream.close();
    return true;
}