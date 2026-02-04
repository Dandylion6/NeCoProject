#pragma once
#include <array>
#include <string>
#include <string_view>


namespace MorseCode
{

constexpr char NULL_CODE = '\x0';
constexpr char CANCEL_CODE = '\x18';
constexpr char BACK_CODE = '\x8';
constexpr float ERROR_MARGIN = 0.48f;


constexpr size_t MORSE_TABLE_SIZE = 512u;
using MorseTable = std::array<char, MORSE_TABLE_SIZE>;


/**
 * Returns an index for the specified morse code sequence.
 * @param morseString Morse code sequence in string form "-..-" for example.
 * @return The index for look-up.
 */
constexpr int GetMorseIndex(const std::string_view morseString)
{
    int morseIndex = 1;
    for (const char character : morseString)
    {
        switch (character)
        {
        case '.': morseIndex *= 2;
            break;
        case '-': morseIndex = morseIndex * 2 + 1;
            break;
        default: break;
        }
    }
    return morseIndex;
}


consteval MorseTable BuildMorseTable()
{
    MorseTable morseTable{ };

    auto Assign = [&](const std::string_view morseString, const char character)
    {
        const int morseIndex = GetMorseIndex(morseString);
        morseTable[morseIndex] = character;
    };

    Assign(".-", 'A');
    Assign("-...", 'B');
    Assign("-.-.", 'C');
    Assign("-..", 'D');
    Assign(".", 'E');
    Assign("..-.", 'F');
    Assign("--.", 'G');
    Assign("....", 'H');
    Assign("..", 'I');
    Assign(".---", 'J');
    Assign("-.-", 'K');
    Assign(".-..", 'L');
    Assign("--", 'M');
    Assign("-.", 'N');
    Assign("---", 'O');
    Assign(".--.", 'P');
    Assign("--.-", 'Q');
    Assign(".-.", 'R');
    Assign("...", 'S');
    Assign("-", 'T');
    Assign("..-",  'U');
    Assign("...-", 'V');
    Assign(".--",  'W');
    Assign("-..-", 'X');
    Assign("-.--", 'Y');
    Assign("--..", 'Z');

    Assign("-----", '0');
    Assign(".----", '1');
    Assign("..---", '2');
    Assign("...--", '3');
    Assign("....-", '4');
    Assign(".....", '5');
    Assign("-....", '6');
    Assign("--...", '7');
    Assign("---..", '8');
    Assign("----.", '9');

    Assign("........", BACK_CODE);
    Assign("...-.-",  CANCEL_CODE);

    return morseTable;
}


constexpr MorseTable TABLE = BuildMorseTable();


inline float DashTime(const float dotTime) noexcept { return dotTime * 3.0f; }
inline float ErrorMargin(const float dotTime) noexcept { return dotTime * ERROR_MARGIN; }
inline float ExitTime(const float dotTime) noexcept { return dotTime * (4.0f + ERROR_MARGIN); }


constexpr bool IsAsciiDigit(const char character) noexcept
{
    return character >= '0' && character <= '9';
}

}
