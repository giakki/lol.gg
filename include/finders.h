#pragma once

#include <string>

#include "./typedefs.h"

/* Find a list of champion builds from the champion.gg homepage. */
ChampionLinks findChampionLinks(const std::string& html);

/* Find a champion's builds from its HTML page. */
ChampionBuilds findBuilds(const std::string& html);
