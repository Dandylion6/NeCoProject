#pragma once
struct EcsContext;
struct GameState;
class ResourceStore;


namespace DoorwayScene
{
	void Build(
		EcsContext& ecsContext,
		GameState& gameState,
		ResourceStore& resourceStore
	);
}