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


struct StreamResult
{
    std::unique_ptr<std::fstream> stream = nullptr;
    bool isExistant = false;
};


StreamResult GetFileStream(std::string&& file)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);

    std::filesystem::path filePath = dataDirectoryPath / file;
    bool fileExisted = std::filesystem::exists(filePath);

    std::unique_ptr<std::fstream> stream = std::make_unique<std::fstream>();
    stream->open(filePath, std::ios::in | std::ios::out);

    return StreamResult 
    {
        std::move(stream),
        fileExisted
    };
}


void Save::SaveSettings(Settings& settings, uint8_t save)
{
    StreamResult result = GetFileStream("settings.json");

    nlohmann::json data;

    data["morse_dot_duration"] = settings.morseSettings.dotTime;

    *result.stream << data.dump(4) << std::endl;
    result.stream->close();
};


void Save::SaveGameState(GameState& gameState, uint8_t save)
{
    StreamResult result = GetFileStream("game_state.json");

    nlohmann::json data;

    data["time"] = gameState.time;
    data["current_scene"] = static_cast<uint8_t>(gameState.currentScene);

    *result.stream << data.dump(4) << std::endl;
    result.stream->close();
}


bool Save::LoadSettings(Settings& settings, uint8_t save)
{
    StreamResult result = GetFileStream("settings.json");

    if (!result.isExistant) return true;
    if (result.stream->peek() == EOF) return true;

    nlohmann::json data = nlohmann::json::parse(*result.stream);

    settings.morseSettings.dotTime = data.at("morse_dot_duration");
    
    Settings::Apply(settings);

    result.stream->close();
    return true;
};


bool Save::LoadGameState(GameState& gameState, uint8_t save)
{
    StreamResult result = GetFileStream("game_state.json");

    if (!result.isExistant || result.stream->peek() == EOF)
    {
        gameState.currentScene = CommsRoom;
        return true;
    }

    nlohmann::json data = nlohmann::json::parse(*result.stream);

    gameState.time = data.at("time");
    gameState.currentScene = static_cast<Scene>(data.at("current_scene"));

    result.stream->close();
    return true;
}