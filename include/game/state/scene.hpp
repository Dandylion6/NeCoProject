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
    Scene scene = NullScene;
    std::array<Scene, 6u> adjacent{ };
    bool isTraversable = false;
};


using Connections = std::array<ConnectionData, SCENE_COUNT>;


consteval Connections BuildConnections()
{
    Connections connections{ };

    const auto AddConnection = [&](
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
        ConnectionData& data = connections.at(current);
        data.adjacent[0u] = up;
        data.adjacent[1u] = down;
        data.adjacent[2u] = left;
        data.adjacent[3u] = right;
        data.adjacent[4u] = front;
        data.adjacent[5u] = back;
        data.isTraversable = true;
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

    const ConnectionData& data = scenes.at(current);
    switch (direction)
    {
    case None: return NullScene;
    case Up: return data.adjacent[0u];
    case Down: return data.adjacent[1u];
    case Left: return data.adjacent[2u];
    case Right: return data.adjacent[3u];
    case Front: return data.adjacent[4u];
    case Back: return data.adjacent[5u];
    }
    return NullScene;
}


inline Direction GetConnectionDirection(const Scene current, const Scene target)
{
    if (current >= scenes.size()) return None;

    const ConnectionData& data = scenes.at(current);
    for (int i = 0; i < 6; ++i)
    {
        const Scene adjacent = data.adjacent[i];
        if (adjacent == target)
            return static_cast<Direction>(i);
    }

    return None;
}

}


