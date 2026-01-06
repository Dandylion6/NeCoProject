#pragma once
#include <cstdint>


namespace Save
{

enum class Result : uint8_t
{
	Success,
    EmptyAddressComponent,
	BadSaveData,
	FileNotFound,
	Failure
};

}


namespace Load
{

enum class Result : uint8_t
{
	Success,
    AddressComponentNotFound,
    ComponentValueNotFound,
	BadRequest,
	MissingFile,
	EmptyFile,
	Failure
};

}