#pragma once
#include "game/state/scene.hpp"


namespace Component
{
    struct MoveRegion final
    {
        // ------ Members ------
        float moveTime = 0.2f;
        Scene nextScene = NullScene;


        // ------ Constructors ------
        explicit MoveRegion(const Scene nextScene, const float moveTime) noexcept :
            moveTime(moveTime), nextScene(nextScene) {}
    };
}