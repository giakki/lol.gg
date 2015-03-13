#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

#include "./typedefs.h"

/* Serialize a json champion and write it to {root}/{name}/Recommended/{title} */
void writeBuild(const boost::filesystem::path& root, const boost::property_tree::ptree& build);

/* Parse a ChampionBuild into a boost::propery_tree, ready to be serialized. */
boost::property_tree::ptree buildToJSON(const std::string& name,
                                        const std::string& role,
                                        const ChampionBuilds& builds);
