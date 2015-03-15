#pragma once

#include <boost/filesystem/path.hpp>

#include <string>

#include "./typedefs.h"

/* Serialize a ChampionBuild and write it to its correct location. */
void writeBuild(const boost::filesystem::path& root,
                const std::string& name,
                const std::string& role,
                const ChampionBuilds& builds);
