#pragma once
#include <cstdint>
struct GameState;
struct Settings;


class Save
{
public:
    static void SaveSettings(Settings& settings, uint8_t save = 0u);
    static void SaveGameState(GameState& gameState, uint8_t save = 0u);

    static bool LoadSettings(Settings& settings, uint8_t save = 0u);
    static bool LoadGameState(GameState& gameState, uint8_t save = 0u);

};