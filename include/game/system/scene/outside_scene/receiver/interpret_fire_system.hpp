#pragma once
struct SystemContext;


namespace Component
{
struct Receiver;
}


namespace Nc
{
class ResourceStore;
};


namespace System::Receiver::Interpret
{

class Fire final
{
public:
	// ------ Members ------
	static constexpr char COMMAND[] = "FIRE";


	// ------ Functions ------

	static void Update(const SystemContext& context);
	static void HandleMessage(const SystemContext& context, Component::Receiver& receiver);
};

}


