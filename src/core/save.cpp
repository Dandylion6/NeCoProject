#include "core/save.hpp"
#include "core/settings.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>


void Save::SaveSettings(Settings& settings)
{
    std::filesystem::path dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    std::filesystem::create_directories(dataDirectoryPath);
    std::filesystem::path settingsFilePath = dataDirectoryPath / "settings.json";

    std::ofstream settingsStream(settingsFilePath);

    nlohmann::json settingsData;

    settingsData["morse_dot_duration"] = settings.morseSettings.dotTime;

    settingsStream << settingsData.dump(4) << std::endl;

    settingsStream.close();
};


bool Save::LoadSettings(Settings& settings, uint8_t save)
{
    std::filesystem::path settingsFilePath = std::filesystem::path(BUILD_DIR_PATH) / "data" / "settings.json";

    std::ifstream settingsStream(settingsFilePath);

    nlohmann::json settingsData = nlohmann::json::parse(settingsStream);

    settings.morseSettings.dotTime = settingsData.at("morse_dot_duration");

    settingsStream.close();
    return true;
};