#pragma once
struct AnomalyState;
struct GameState;


struct StatesContext
{
    // ------ Members ------
    AnomalyState& anomaly;
    GameState& game;


    // ------ Constructors ------

    explicit StatesContext(AnomalyState& anomaly, GameState& game) noexcept
        : anomaly(anomaly), game(game) { };
};
