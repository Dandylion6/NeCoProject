#pragma once
#include <string>
#include <utility>


namespace Component
{
    struct Address
    {
        std::string address = "";
        Address(std::string&& address): address(std::move(address)) { };
    };
    
}