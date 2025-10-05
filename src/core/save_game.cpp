#include "core/game_state.hpp"
#include "core/save_game.hpp"
#include "core/scene.hpp"
#include "core/settings.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


namespace Save
{


    static bool SaveGameState(GameState& gameState, std::ofstream& stream)
    {
        stream.write(reinterpret_cast<char*>(&gameState.anomalyState.attractionPercentage), sizeof(float));
        stream.write(reinterpret_cast<char*>(&gameState.time), sizeof(float));
        uint8_t rawScene = gameState.currentScene;
        stream.write(reinterpret_cast<char*>(&rawScene), sizeof(rawScene));

        stream.close();
        return true;
    }


    static bool LoadGameState(GameState& gameState, std::ifstream& stream)
    {
        stream.read(reinterpret_cast<char*>(&gameState.anomalyState.attractionPercentage), sizeof(float));
        stream.read(reinterpret_cast<char*>(&gameState.time), sizeof(float));
        uint8_t rawScene = static_cast<uint8_t>(CommsDesk);
        stream.read(reinterpret_cast<char*>(&rawScene), sizeof(rawScene));
        gameState.currentScene = static_cast<Scene>(rawScene);

        stream.close();
        return true;
    }

}


bool Save::SaveGame(entt::registry& registry, GameState& gameState)
{
    if (gameState.save.empty()) return false; // No specified save.

    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ofstream stream(dataDirectoryPath / (gameState.save + ".save"), std::ios::out | std::ios::binary);

    Save::SaveGameState(gameState, stream);
    return true;
}


bool Save::LoadGame(entt::registry& registry, GameState& gameState)
{
    if (gameState.save.empty()) return false; // No save specified to load from.

    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ifstream stream(dataDirectoryPath / (gameState.save + ".save"), std::ios::in | std::ios::binary);

    Save::LoadGameState(gameState, stream);
    return true;
}