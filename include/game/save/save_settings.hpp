#pragma once
struct Settings;


namespace Save
{
    void SaveSettings(Settings& settings);
    bool LoadSettings(Settings& settings);
}