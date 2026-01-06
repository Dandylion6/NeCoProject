#pragma once
#include "entt/entity/fwd.hpp"
namespace Nc { class ResourceStore; };


namespace Entity
{

class Note final
{
public:
	static entt::entity Create(entt::registry& registry, Nc::ResourceStore& resourceStore) noexcept;

};

}
