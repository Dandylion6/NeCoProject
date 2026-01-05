#pragma once 
#include "entt/entity/fwd.hpp"
namespace Component 
{
	namespace Action { struct Toggle; };
	struct Radar; 
};
namespace Nc { class ResourceStore; };


namespace Object
{

class Radar final
{
public:
	static void Create(
		entt::registry& registry, 
		Nc::ResourceStore& resourceStore
	) noexcept;

private:
	class Path final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry,
			Nc::ResourceStore& resourceStore
		) noexcept;
		
	};


	class Artillery final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry,
			Nc::ResourceStore& resourceStore
		) noexcept;

	};


	class ErrorWarning final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry
		) noexcept;

	};


	class RecalibrationText final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry
		) noexcept;

	};


	class PowerButton final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry,
			Component::Radar& radar,
			Component::Action::Toggle& toggle
		) noexcept;

	};

};

}