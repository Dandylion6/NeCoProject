#pragma once
#include <string>
#include <utility>


namespace Component
{
    /**
     * @brief Used to ID a specfic entity.
     * 
     * Saving and loading relies on an entity's address component
     * to match the save data to the correct entity.
     * 
     * Usage example:
     * ```cpp
     * registry.emplace<Component::Action::Toggle>(entity, ...);
     * registry.emplace<Component::Address>(entity, "comms_room_radar");
     * ```
     */
    struct Address
    {
        // ------ Members ------

        std::string address = "";


        // ------ Constructors ------

        constexpr Address(std::string&& address): 
            address(std::move(address)) 
        { };
    };
    
}