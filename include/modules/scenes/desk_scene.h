#pragma once
struct EcsContext;
struct GameState;
class ResourceStore;


namespace DeskScene
{
	void Build(
		EcsContext& ecsContext, 
		GameState& gameState,
		ResourceStore& resourceStore
	);
}