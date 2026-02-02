#pragma once
#include "game/state/scene.hpp"


namespace Component
{
struct TweenCollection;
}


struct SystemContext;
struct SceneContext;


namespace System::UI
{

class MoveTransition final
{
public:
    // ------ Functions ------
    static void Update(const SystemContext& context);
    static void StartMoveScene(const SceneContext& context, float moveTime) noexcept;
    static void InstantTransition(const SceneContext& context, Scene nextScene) noexcept;

private:
    // ------ Functions ------
    static void TransitionDown(const SystemContext& context, Component::TweenCollection& collection);
    static void TransitionUp(const SystemContext& context, const Component::TweenCollection& collection);
};

}
