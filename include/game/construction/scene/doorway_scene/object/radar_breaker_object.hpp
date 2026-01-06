#pragma once
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Object
{

class RadarBreaker final
{
public:
	static void Create(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore, 
		entt::entity radar
	) noexcept;

private:
	class LeverBase final
	{
	public:
		static entt::entity Create(
			entt::registry& registry,
			Nc::ResourceStore& resourceStore
		) noexcept;

	};


	class LeverHandle final
	{
	public:
		static entt::entity Create(
			entt::registry& registry,
			Nc::ResourceStore& resourceStore,
			entt::entity radar
		) noexcept;

	};

};

}