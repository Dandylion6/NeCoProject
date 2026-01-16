#include "game/construction/scene/outside_scene/outside_scene.hpp"
#include "game/construction/scene/outside_scene/entity/artillery_entity.hpp"
#include "game/contexts/build_context.hpp"


void Structure::OutsideScene::Build(const BuildContext& context) noexcept
{
	Entity::Artillery::Create(context.registry);
}