#include "systems/object/receiver/artillery_control_system.h"
#include <string>


const std::string ArtilleryControlSystem::COMMAND = "AIM";


void ArtilleryControlSystem::CheckReceivedMessage(const std::string& message)
{
	if (message == COMMAND)
	{

	}
};