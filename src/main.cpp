#include <boost/filesystem/path.hpp>

#include <iostream>

#include "./colors.h"
#include "./lolitems.h"
#include "./strings.h"
#include "./utility.h"

int main(int argc, char* argv[]) {
    /* Print banner */
    std::cout << Color::Lightmagenta;
    for (const auto& line : strings::banner) {
        std::cout << alignCenter(line) << std::endl;
    }
    std::cout << std::endl;

    /* Initialize CURL */
    CurlGlobal cg;
    if (cg.status != CURLE_OK) {
        std::cout << "Failed to initialize CURL";
        return 1;
    }

    /* Calculate output directory */
    boost::filesystem::path root;
    if (argc == 2) {
        root = argv[1];
    } else {
        root = boost::filesystem::path(argv[0]).remove_filename() / "Config/Champions";
    }

    /* Run program */
    auto res = runProgram(root);
    if (res == 0) {
        std::cout << Color::Grey << strings::finished;
    }

    return res;
}
