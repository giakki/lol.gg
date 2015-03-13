#include "./serialize.h"

#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/json_parser.hpp>

/* Format: {
 *    "champion": name,
 *    "title": role,
 *    "blocks": [{
 *        "type": "build.first (title)"
 *        "items": [{ "count": 1, "id": "item.id" }]
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
boost::property_tree::ptree buildToJSON(const std::string& name,
                                        const std::string& role,
                                        const ChampionBuilds& builds) {
    boost::property_tree::ptree build_tree;

    build_tree.put("champion", name);
    build_tree.put("title", role);

    boost::property_tree::ptree blocks_array;
    for (size_t i = 0; i < builds.size(); ++i) {
        boost::property_tree::ptree items_array;

        std::for_each(std::begin(builds[i].second),
                      std::end(builds[i].second),
                      [&items_array](const std::pair<std::string, size_t>& value) {
            boost::property_tree::ptree child;
            child.put("id", value.first);
            child.put("count", value.second);
            items_array.push_back({ "", child });
        });

        boost::property_tree::ptree child;
        child.put("type", builds[i].first);
        child.push_back({ "items", items_array });
        blocks_array.push_back({ "", child });
    }
    build_tree.push_back({ "blocks", blocks_array });

    /* Other needed values */
    build_tree.put("map", "any");
    build_tree.put("isGlobalForChampions", false);
    // build_tree.put("associatedChampions", boost::property_tree::ptree());
    build_tree.put("priority", false);
    build_tree.put("mode", "any");
    build_tree.put("isGlobalForMaps", true);
    // build_tree.put("associatedMaps", boost::property_tree::ptree());
    build_tree.put("type", "custom");
    build_tree.put("sortrank", 1);

    return build_tree;
}

void writeBuild(const boost::filesystem::path& root,
                const boost::property_tree::ptree& build_json) {
    auto name = build_json.get<std::string>("champion");
    auto role = build_json.get<std::string>("title");
    auto build_destination = root / name / "Recommended";
    auto filename = build_destination / (role + ".json");

    boost::filesystem::create_directories(build_destination);

    std::ofstream fout(filename.string());
    if (!fout.is_open()) {
        throw std::runtime_error(filename.string() + " : unable to open.");
    }
    boost::property_tree::write_json(fout, build_json);
}
