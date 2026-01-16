#pragma once
#include <cstdint>
#include <string>


namespace Nc::Cursor
{

enum Type : uint8_t
{
    Standard,
    Clickable,
    Grab,
    Move,
};


constexpr uint8_t GetCursorPriority(const Type type) noexcept
{
    switch (type)
    {
    case Clickable: return 1;
    case Grab: return 2;
    case Move: return 3;
    default: return 0;
    }
}


/**
 * @brief Assigns a cursor type if it has higher priority than the current one.
 * @param current The currently selected cursor type.
 * @param candidate The cursor type being considered.
 */
inline void AssignIfHigherPriority(Type& current, const Type candidate) noexcept
{
    current = GetCursorPriority(candidate) > GetCursorPriority(current) ? candidate : current;
}


inline std::string GetCursorPath(const Type type) noexcept
{
    // TODO: Add custom cursors.
    switch (type)
    {
    case Standard: return "Standard";
    case Clickable: return "Clickable";
    case Grab: return "Grab";
    case Move: return "Move";
    default: return "unknown";
    }
}


}
