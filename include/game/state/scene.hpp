#pragma once
#include <array>
#include <cstdint>

#include "core/data/vector2.hpp"


enum Scene: uint8_t
{
    NullScene,
    CommsRoom,
    RadarScene,
    CommsDesk,
    Doorway,
    Outside,
    SCENE_COUNT,
};


struct ConnectionData final
{
    std::array<Scene, 6u> adjacent{ };
    bool isTraversable = false;
};


using Connections = std::array<ConnectionData, SCENE_COUNT>;


consteval Connections BuildConnections()
{
    Connections connections{ };

    auto AddConnection = [&](
        const Scene current,
        const Scene up,
        const Scene down,
        const Scene left,
        const Scene right,
        const Scene front,
        const Scene back
    )
    {
        if (connections.size() <= current) return;
        auto& [adjacent, isTraversable] = connections.at(current);
        adjacent[0u] = up;
        adjacent[1u] = down;
        adjacent[2u] = left;
        adjacent[3u] = right;
        adjacent[4u] = front;
        adjacent[5u] = back;
        isTraversable = true;
    };

    AddConnection(CommsRoom, NullScene, CommsDesk, NullScene, Doorway, NullScene, NullScene);
    AddConnection(CommsDesk, CommsRoom, NullScene, NullScene, NullScene, NullScene, NullScene);
    AddConnection(Doorway, NullScene, NullScene, CommsRoom, NullScene, NullScene, NullScene);

    return connections;
}


constexpr Connections scenes = BuildConnections();


namespace SceneMath
{

inline Scene GetConnectedScene(const Scene current, const Direction direction)
{
    if (current >= scenes.size()) return NullScene;

    const auto& [adjacent, isTraversable] = scenes.at(current);
    if (!isTraversable) return NullScene;

    switch (direction)
    {
    case None: return NullScene;
    case Up: return adjacent[0u];
    case Down: return adjacent[1u];
    case Left: return adjacent[2u];
    case Right: return adjacent[3u];
    case Front: return adjacent[4u];
    case Back: return adjacent[5u];
    }
    return NullScene;
}


inline Direction GetConnectionDirection(const Scene current, const Scene target)
{
    if (current >= scenes.size()) return None;

    const auto& [adjacent, isTraversable] = scenes.at(current);
    if (!isTraversable) return None;

    for (int i = 0; i < 6; ++i)
    {
        const Scene adjacentScene = adjacent.at(i);
        if (adjacentScene == target)
            return static_cast<Direction>(i);
    }

    return None;
}

}


