#pragma once
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Entity
{

	class MainMenuBackground final
	{
	public:
		static const entt::entity Create(
			entt::registry& registry,
			Nc::ResourceStore& resourceStore
		) noexcept;

	};

}