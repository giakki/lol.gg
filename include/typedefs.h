#pragma once

#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

/* Temporary wrapper for a champion link */
typedef std::tuple<std::string, std::string, std::string> LinkWrapper;
/* A collection of champion links */
typedef std::set<LinkWrapper> ChampionLinks;
/* An item has a count and an id */
typedef std::pair<std::string, size_t> Item;
/* A build is defined by a name and a number of items. */
typedef std::pair<std::string, std::vector<Item> > Build;

/* Each champion has 4 builds: most wins and most common starters/core */
const size_t ExpectedNumBuilds = 4; //-V112
typedef std::vector<Build> ChampionBuilds;

/* Forward declarations */
typedef void CURL;

struct GumboInternalNode;
typedef GumboInternalNode GumboNode;

struct GumboInternalOutput;
typedef GumboInternalOutput GumboOutput;

/* RAII typedefs */
typedef std::unique_ptr<CURL, void (*)(CURL*)> CurlManaged;

typedef std::unique_ptr<GumboOutput, void (*)(GumboOutput*)> GumboOutputManaged;
