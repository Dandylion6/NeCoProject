#pragma once
#include "game/save/save_result.hpp"
struct Settings;


namespace Save
{

Result SettingsToDisk(const Settings& settings);

}


namespace Load
{

Result SettingsFromDisk(Settings& settings);

}