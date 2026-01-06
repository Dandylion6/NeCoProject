#pragma once
#include "core/data/vector2.hpp"
#include "entt/entity/fwd.hpp"
#include "game/component/scene/comms_scene/morse_components.hpp"
namespace Nc { class ResourceStore; };


namespace Object
{

class MorseMonitor final
{
public:
	static constexpr Nc::Vector2f GAUGE_SIZE = Nc::Vector2f(98.0f, 8.0f);
	static constexpr Nc::Vector2f POSITION = Nc::Vector2f(280.0f, 440.0f);

	
	static void Create(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore
	) noexcept;

private:
	class Gauge final
	{
	public:
		static entt::entity Create(
			entt::registry& registry
		) noexcept;

	};


	class Pointer final
	{
	public:
		static entt::entity Create(
			entt::registry& registry
		) noexcept;

	};


	class Region final
	{
	public:
		static entt::entity Create(
			entt::registry& registry, 
			Component::Morse::MonitorRegion::Region region
		) noexcept;

	};

};

}