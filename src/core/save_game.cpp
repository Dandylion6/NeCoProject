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
    static void SaveGameState(GameState& gameState, const std::string& save)
    {
        std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
        if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
        std::ofstream stream(dataDirectoryPath / (save + ".save"));

        // TODO: Implement GateState saving

        stream.close();
    }


    static bool LoadGameState(GameState& gameState, const std::string& save)
    {
        std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
        if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
        std::ifstream stream(dataDirectoryPath / (save + ".save"));

        // TODO: Implement GateState loading

        stream.close();
        return true;
    }
}


void Save::SaveGame(entt::registry& registry, GameState& gameState, const std::string& save)
{
    Save::SaveGameState(gameState, save);
}


void Save::LoadGame(entt::registry& registry, GameState& gameState, const std::string& save)
{
    Save::LoadGameState(gameState, save);
}