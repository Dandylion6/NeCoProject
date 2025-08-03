#include "core/save.hpp"
#include "core/game_state.hpp"
#include "core/scene.hpp"
#include "core/settings.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>




void Save::SaveSettings(Settings& settings, uint8_t save)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ofstream stream(dataDirectoryPath / "settings.json");

    nlohmann::json data;

    data["morse_dot_duration"] = settings.morseSettings.dotTime;

    stream << data.dump(4) << std::endl;
    stream.close();
};


void Save::SaveGameState(GameState& gameState, uint8_t save)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ofstream stream(dataDirectoryPath / "game_state.json");

    nlohmann::json data;

    data["time"] = gameState.time;
    data["current_scene"] = static_cast<uint8_t>(gameState.currentScene);

    stream << data.dump(4) << std::endl;
    stream.close();
}


bool Save::LoadSettings(Settings& settings, uint8_t save)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ifstream stream(dataDirectoryPath / "settings.json");

    if (!std::filesystem::exists(dataDirectoryPath / "settings.json")) return true;
    if (stream.peek() == EOF) return true;

    nlohmann::json data = nlohmann::json::parse(stream);

    settings.morseSettings.dotTime = data.at("morse_dot_duration");
    
    Settings::Apply(settings);

    stream.close();
    return true;
};


bool Save::LoadGameState(GameState& gameState, uint8_t save)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ifstream stream(dataDirectoryPath / "game_state.json");

    if (!std::filesystem::exists(dataDirectoryPath / "settings.json") || stream.peek() == EOF)
    {
        gameState.currentScene = CommsRoom;
        return true;
    }

    nlohmann::json data = nlohmann::json::parse(stream);

    gameState.time = data.at("time");
    gameState.currentScene = static_cast<Scene>(data.at("current_scene"));

    stream.close();
    return true;
}