#include "./serialize.h"

#include <boost/filesystem/operations.hpp>

#include <fstream>

#include "./strings.h"

/* Helper functions to serialize the builds into JSON. Look below for the desired format.
 * We could use a library, but boost::property_tree::ptree's json is broken, and since we use it
 * only here it makes no sense adding a dependency.
 */
std::ostream& operator<<(std::ostream& os, const Item& item);
std::ostream& operator<<(std::ostream& os, const Build& build);
std::ostream& operator<<(std::ostream& os, const ChampionBuilds& builds);

/* { "id": "item.id"  "count": item.count } */
std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << R"({ "id": ")" << item.first << R"(", "count": )" << item.second << "}"; //-V128
    return os;
}

/* {
 *     "type": "build.first",
 *     "items": [build.second]
 * }
 */
std::ostream& operator<<(std::ostream& os, const Build& build) {
    os << R"({ "type": ")" << build.first << R"(", "items": [)";
    for (auto it = std::begin(build.second); it != std::end(build.second); ++it) {
        if (it != std::begin(build.second)) {
            os << ",";
        }
        os << (*it);
    }
    os << "]}";

    return os;
}

/* "blocks": [ each build ], */
std::ostream& operator<<(std::ostream& os, const ChampionBuilds& builds) {
    os << R"("blocks": [)";
    for (auto it = std::begin(builds); it != std::end(builds); ++it) {
        if (it != std::begin(builds)) {
            os << ",";
        }
        os << (*it);
    }
    os << "],";

    return os;
}

/* Format: {
 *    "champion": name,
 *    "title": role,
 *    "blocks": [{
 *        "type": "build.first",
 *        "items": [{ "count": item.count, "id": "item.id" }]
 *    }],
 *    "map": "any",
 *    "isGlobalForChampions": false,
 *    "associatedChampions": [],
 *    "priority": false,
 *    "mode": "any",
 *    "isGlobalForMaps": true,
 *    "associatedMaps": [],
 *    "type": "custom",
 *    "sortrank": 1
 * }
 */
void writeBuild(const boost::filesystem::path& root,
                const std::string& name,
                const std::string& role,
                const ChampionBuilds& builds) {
    auto build_destination = root / name / "Recommended";
    auto filename = build_destination / (role + ".json");

    /* Create destination folder */
    boost::filesystem::create_directories(build_destination);

    std::ofstream fout(filename.string());
    if (!fout.is_open()) {
        throw std::runtime_error(filename.string() + " : unable to open.");
    }

    /* Serialize build */
    fout << R"({ "champion": ")" << name << R"(", "title": ")" << role << R"(",)";
    fout << builds;
    for (auto& line : strings::json_epilogue) {
        fout << line;
    }
}
