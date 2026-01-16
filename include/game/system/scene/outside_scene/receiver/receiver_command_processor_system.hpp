#pragma once
#include <string>


struct SystemContext;


namespace Component
{
struct Receiver;
}


namespace Nc
{
class ResourceStore;
};


namespace System::Receiver
{

class CommandProcessor final
{
public:
	// ------ Functions ------
	static void Update(const SystemContext& context);

private:
	// ------ Functions ------
	static bool CheckCharacterCommand(const SystemContext& context, Component::Receiver& receiver);
	static void ProcessMessage(const SystemContext& context, Component::Receiver& receiver, const std::string& message);
	static TransmissionContext DetermineContext(const std::string& message);
};

}


