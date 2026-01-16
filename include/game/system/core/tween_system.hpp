#pragma once
struct SystemContext;


namespace Nc
{
struct Tween;
}


namespace Component
{
struct TweenCollection;
}


namespace System
{

class Tween final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context);

private:
	// ------ Functions ------
	static void UpdateTweenCollection(Component::TweenCollection& collection, float deltaTime);
	static void TweenEnded(Nc::Tween& tween);

};

}

