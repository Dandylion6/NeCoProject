#pragma once
#include "entt/entity/fwd.hpp"
struct SystemContext;


namespace Component
{
struct Receiver;
struct Radar;


namespace Action
{
struct Toggle;
}
}


namespace Nc
{
class ResourceStore;
};


namespace System::Receiver::Interpret
{

class Recalibration final
{
public:
	// ------ Members ------
	static constexpr char COMMAND[] = "OPTSIG";


	// ------ Functions ------

	static void HandleMessage(const SystemContext& context, Component::Receiver& receiver);
	static void Update(entt::registry& registry, float deltaTime);

private:
	// ------ Functions ------
	static void ConfirmCommand(const SystemContext& context, Component::Receiver& receiver);
	static void Completed(Component::Radar& radar, Component::Action::Toggle& toggle);

};

}

