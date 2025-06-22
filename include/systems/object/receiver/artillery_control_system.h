#pragma once
#include <cstdint>
#include <string>

class ArtilleryControlSystem
{
public:
	static const std::string COMMAND;
	static void CheckReceivedMessage(const std::string& message);

};