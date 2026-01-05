#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
struct GameState;
namespace Nc { class ResourceStore; };


namespace Object
{

class RestartMenuBackground final
{
public:
	static void Create(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore, 
		Nc::Vector2f windowSize
	) noexcept;

private:
	class FrontTexture final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry, 
			Nc::ResourceStore& resourceStore
		) noexcept;

	};


	class Backdrop final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry, 
			Nc::Vector2f windowSize
		) noexcept;

	};

};

}