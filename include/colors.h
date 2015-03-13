#pragma once

#include <iosfwd>

enum class Color {
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    Lightgrey,
    /* These are actually bold on Linux */
    Grey,
    Lightblue,
    Lightgreen,
    Lightcyan,
    Lightred,
    Lightmagenta,
    Yellow,
    White
};

std::ostream& operator<<(std::ostream& stream, const Color& color);
