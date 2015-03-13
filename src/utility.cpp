#include "./utility.h"

#include <curl/curl.h>
#include <gumbo.h>
#include <boost/optional/optional.hpp>

#include <ostream>

#include "./strings.h"

/* Get number of columns in console. Platform dependent. */
boost::optional<size_t> getConsoleWidth();

/* Windows console functions */
#if defined(_WIN32)
#include <windows.h>

void setTitle(const std::string& str) {
    SetConsoleTitle(std::wstring(std::begin(str), std::end(str)).c_str());
}

boost::optional<size_t> getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    auto handle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!GetConsoleScreenBufferInfo(handle, &csbi)) {
        return boost::none;
    } else {
        return csbi.srWindow.Right - csbi.srWindow.Left;
    }
}

std::ostream& clearLine(std::ostream& stream) {
    auto opt_width = getConsoleWidth();
    size_t width = 79;
    if (width) {
        width = *opt_width;
    }
    return stream << "\r" << std::string(width - 1, ' ') << "\r" << std::flush;
}

#else

/* Unix console functions */
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void setTitle(const std::string&) {
}

boost::optional<size_t> getConsoleWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    return w.ws_col;
}

/* On linux, just push a new line */
std::ostream& clearLine(std::ostream& stream) {
    return stream << std::endl;
}

#endif

std::string alignCenter(const std::string& str) {
    auto width = getConsoleWidth();
    if (width) {
        return std::string((*width - str.length()) / 2, ' ') + str;
    } else {
        return str;
    }
}

GumboOutputManaged gumboParse(const char* html, size_t size) {
    return GumboOutputManaged(
        gumbo_parse_with_options(&kGumboDefaultOptions, html, size),
        [](GumboOutput* output) { gumbo_destroy_output(&kGumboDefaultOptions, output); });
}
