#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>


namespace MorseCode
{
    constexpr int8_t NULL_CODE = '\x0';
    constexpr int8_t CANCEL_CODE = '\x18';
    constexpr int8_t BACK_CODE = '\x8';

	constexpr float SHORT_DURATION = 0.15f;
	constexpr float LONG_DURATION = 3.0f * SHORT_DURATION;

	enum Pulse
	{
        Invalid = 0,
		Short = 1,
		Long = 2
	};

    const std::unordered_map<std::string, int8_t> TABLE {
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


    inline int8_t GetChar(const std::string& code)
    {
        if (TABLE.find(code) == TABLE.end()) return MorseCode::NULL_CODE;
        return TABLE.at(code);
    }
}