#pragma once
#include <array>
#include <cstdint>


namespace Component
{

struct Journal final
{
    // ------ Types ------

    struct Page final
    {
        enum PageType : uint8_t
        {
            NotFound, Found, Partial
        };

        PageType type = NotFound;

        Page() = default;
        explicit Page(const PageType type) : type(type) {}
    };


	// ------ Members ------

    static constexpr uint8_t MAX_PAGES = 5;

    std::array<Page, MAX_PAGES> pages{ };
	uint8_t index = 0;
    bool pageTurned = false;


    // ------ Constructors ------

    Journal() = default;
    explicit Journal(const uint8_t index) : index(index) {}
};

}


