#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>


namespace MorseCode
{

constexpr char NULL_CODE = '\x0';
constexpr char CANCEL_CODE = '\x18';
constexpr char BACK_CODE = '\x8';
constexpr float ERROR_MARGIN = 0.48f;

enum Pulse: uint8_t
{
    Invalid,
	Short,
	Long,
};

const std::unordered_map<std::string, char> TABLE {
    { ".-", 'A' },  { "-...", 'B' },  { "-.-.", 'C' },
      { "-..", 'D' },     { ".", 'E' },  { "..-.", 'F' },
      { "--.", 'G' },  { "....", 'H' },    { "..", 'I' },
     { ".---", 'J' },   { "-.-", 'K' },  { ".-..", 'L' },
       { "--", 'M' },    { "-.", 'N' },   { "---", 'O' },
     { ".--.", 'P' },  { "--.-", 'Q' },   { ".-.", 'R' },
      { "...", 'S' },     { "-", 'T' },   { "..-", 'U' },
     { "...-", 'V' },   { ".--", 'W' },  { "-..-", 'X' },
     { "-.--", 'Y' },  { "--..", 'Z' },
    { "-----", '0' }, { ".----", '1' }, { "..---", '2' },
    { "...--", '3' }, { "....-", '4' }, { ".....", '5' },
    { "-....", '6' }, { "--...", '7' }, { "---..", '8' },
    { "----.", '9' },
    { "........", MorseCode::BACK_CODE }, { "-.-.-.-",  MorseCode::CANCEL_CODE }
};


inline float DashTime(const float dotTime) noexcept { return dotTime * 3.0f; };
inline float ErrorMargin(const float dotTime) noexcept { return dotTime * ERROR_MARGIN; };
inline float ExitTime(const float dotTime) noexcept { return dotTime * (4.0f + ERROR_MARGIN); };


inline char GetChar(const std::string& code) noexcept
{
    if (!TABLE.contains(code)) return NULL_CODE;
    return TABLE.at(code);
}

}