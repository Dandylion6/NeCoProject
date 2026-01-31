#include "game/save/save_settings.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

#include "game/state/settings.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"


Save::Result Save::SettingsToDisk(const Settings& settings)
{
    const auto dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ofstream stream(dataDirectoryPath / "settings.json");

    nlohmann::json data;

    data["morse_dot_duration"] = settings.morseSettings.dotTime;

    stream << data.dump(4) << std::endl;
    stream.close();
    return Result::Success;
}


Load::Result Load::SettingsFromDisk(Settings& settings)
{
    const auto dataDirectoryPath = std::filesystem::path(BUILD_DIR_PATH) / "data";
    if (!std::filesystem::is_directory(dataDirectoryPath)) std::filesystem::create_directories(dataDirectoryPath);
    std::ifstream stream(dataDirectoryPath / "settings.json");

    if (!std::filesystem::exists(dataDirectoryPath / "settings.json")) return Result::MissingFile;
    if (stream.peek() == EOF) return Result::EmptyFile;

    nlohmann::json data = nlohmann::json::parse(stream);

    settings.morseSettings.dotTime = data.at("morse_dot_duration");

    stream.close();
    return Result::Success;
}
