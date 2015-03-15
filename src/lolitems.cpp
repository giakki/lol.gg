#include "./lolitems.h"

#include <boost/range/adaptor/indexed.hpp>
#include <boost/filesystem/operations.hpp>

#include <chrono>
#include <iostream>
#include <thread>

#include "./colors.h"
#include "./finders.h"
#include "./http.h"
#include "./serialize.h"
#include "./utility.h"

int runProgram(const boost::filesystem::path& root) {
    /* We will reuse this for all of the cUrl operations */
    HttpClient client;
    ChampionLinks champion_links;

    try {
        client.init();
    } catch (const std::exception& e) {
        std::cout << Color::Red << e.what() << std::endl;
        return 1;
    }

    /* Get champion links */
    std::cout << Color::Cyan << "Getting champion list... ";
    try {
        champion_links = findChampionLinks(client.get("http://champion.gg"));
    } catch (const std::exception& e) {
        std::cout << Color::Red << e.what() << std::endl;
        return 1;
    }

    /* Prepare directory tree */
    boost::filesystem::create_directories(root);
    size_t num_champions = champion_links.size();

    /* Iterate over champions */
    std::cout << "done. Found " << num_champions << " builds." << std::endl;
    for (const auto&& element : champion_links | boost::adaptors::indexed(1)) {
        /* We wait at least 1 second between each request to not thrash the champion.gg server */
        auto next_request = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        setTitle(std::string("lolitems (") + std::to_string(element.index()) + "/" +
                 std::to_string(num_champions) + ")");
        std::cout << Color::White << "(" << element.index() << "/" << num_champions << ") "
                  << Color::Cyan << std::get<0>(element.value()) << " "
                  << std::get<1>(element.value()) << "... ";
        /* Get the builds for a specific champion */
        ChampionBuilds builds;
        try {
            /* Parse the html */
            builds = findBuilds(
                client.get(std::string("http://champion.gg") + std::get<2>(element.value())));
            /* Save the build to disk. */
            writeBuild(root, std::get<0>(element.value()), std::get<1>(element.value()), builds);
        } catch (const std::exception& e) {
            /* Simply skip over to next champion */
            std::cout << Color::Red << e.what() << std::endl;
        }
        /* Sleep for the required time */
        std::this_thread::sleep_until(next_request);
        /* Clear the output */
        std::cout << clearLine;
    }
    return 0;
}
