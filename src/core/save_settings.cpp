#include "core/game_state.hpp"
#include "core/save_settings.hpp"
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


void Save::SaveSettings(Settings& settings)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ofstream stream(dataDirectoryPath / "settings.json");

    nlohmann::json data;

    data["morse_dot_duration"] = settings.morseSettings.dotTime;

    stream << data.dump(4) << std::endl;
    stream.close();
};


bool Save::LoadSettings(Settings& settings)
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