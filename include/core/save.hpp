#pragma once
#include <cstdint>
struct Settings;


class Save
{
public:
    static void SaveSettings(Settings& settings);

    static bool LoadSettings(Settings& settings, uint8_t save = 0u);

};