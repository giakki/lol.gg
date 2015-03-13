#include "./colors.h"

#include <ostream>

#if !defined(_WIN32)
#include <boost/range/algorithm/find_if.hpp>

#pragma pack(push, 1)
constexpr struct {
    Color color;
    const char* string;
} color_map[] = { { Color::Black, "\033[22;30m" },
                  { Color::Red, "\033[22;31m" },
                  { Color::Green, "\033[22;32m" },
                  { Color::Brown, "\033[22;33m" },
                  { Color::Blue, "\033[22;34m" },
                  { Color::Magenta, "\033[22;35m" },
                  { Color::Cyan, "\033[22;36m" },
                  { Color::Grey, "\033[22;37m" },
                  { Color::Lightgrey, "\033[01;30m" },
                  { Color::Lightred, "\033[01;31m" },
                  { Color::Lightgreen, "\033[01;32m" },
                  { Color::Yellow, "\033[01;33m" },
                  { Color::Lightblue, "\033[01;34m" },
                  { Color::Lightmagenta, "\033[01;35m" },
                  { Color::Lightcyan, "\033[01;36m" },
                  { Color::White, "\033[01;37m" } };
#pragma pack(pop)
#else
#include <Windows.h>
#endif

std::ostream& operator<<(std::ostream& stream, const Color& color) {
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
#else
    /* Find the color in color_map */
    auto match = boost::find_if(
        color_map, [&color](decltype(*std::begin(color_map)) pair) { return pair.color == color; });

    stream << match->string;
#endif
    return stream;
}
