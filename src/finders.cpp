#include "./finders.h"

#include <gumbo.h>

#include <boost/format.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/remove_if.hpp>
/* It looks like regexes are still broken in the linux stdlib */
#include <boost/regex.hpp>

#include <iostream>

#include "./colors.h"
#include "./strings.h"
#include "./utility.h"

/* Traverse a node tree, calling {visitor} for each element node. */
void traverse(const GumboNode* node, const std::function<void(const GumboNode*)>& visitor);

/* Some items are not available */
std::string parseItem(const std::string& item);

/* Given a div wrapper, find the children imgs and extract the item id from the src attribute */
std::vector<Item> findItems(const GumboNode* wrapper);

/* Given a node, find all the child divs with a class of build-wapper */
std::vector<const GumboNode*> findBuildWrappers(const GumboNode* root);

void traverse(const GumboNode* node, const std::function<void(const GumboNode*)>& visitor) {
    if (node->type == GUMBO_NODE_ELEMENT) {
        /* Apply visitor to node... */
        visitor(node);
        /* ...and to all children, depth first */
        const GumboVector* children = &node->v.element.children;
        for (size_t i = 0; i < static_cast<size_t>(children->length); ++i) {
            traverse(static_cast<const GumboNode*>(children->data[i]), visitor);
        }
    }
    return;
}

std::string parseItem(const std::string& item) {
    /* Biscuit -> Health potion */
    if (item == "2010") {
        return "2003";
    }
    return item;
}

std::vector<Item> findItems(const GumboNode* wrapper) {
    std::vector<Item> items;
    /* Example item src: //ddragon.leagueoflegends.com/cdn/5.5.2/img/item/3047.png */
    boost::regex item_regex("([0-9]+).png$", boost::regex::optimize);

    /* Find only images with a src attribute matching the regex */
    traverse(wrapper, [&items, &item_regex](const GumboNode* node) {
        if (node->v.element.tag == GUMBO_TAG_IMG) {
            /* Extract href */
            auto src = gumbo_get_attribute(&node->v.element.attributes, "src")->value;
            /* Find the matching item id and add it to the build */
            boost::match_results<const char*> item_match;
            if (boost::regex_search(src, item_match, item_regex)) {
                auto id = parseItem(item_match[1].str());
                /* If we already added the item, just increase its count. Otherwise, insert it with
                 * a count of 1 */
                auto match =
                    boost::find_if(items, [&id](const Item& item) { return item.first == id; });
                if (match != std::end(items)) {
                    ++(*match).second;
                } else {
                    items.emplace_back(id, 1);
                }
            } else {
                throw std::runtime_error("Error finding items.");
            }
        }
    });

    return items;
}

std::vector<const GumboNode*> findBuildWrappers(const GumboNode* root) {
    std::vector<const GumboNode*> wrappers;

    /* Find only divs with a class attribute of build-wrapper */
    traverse(root, [&wrappers](const GumboNode* node) {
        if (node->v.element.tag == GUMBO_TAG_DIV) {
            auto cls_attr = gumbo_get_attribute(&node->v.element.attributes, "class");
            if ((cls_attr != nullptr) && std::strstr(cls_attr->value, "build-wrapper") != nullptr) {
                wrappers.push_back(node);
            }
        }
    });

    return wrappers;
}

std::set<LinkWrapper> findChampionLinks(const std::string& html) {
    auto output = gumboParse(html.c_str(), html.size());
    std::set<LinkWrapper> links;
    /* Example valid href: "/champion/Aatrox/Top" */
    boost::regex champion_regex("^/champion/([a-z]+)/([a-z]+)$",
                                boost::regex::icase | boost::regex::optimize);

    /* Find only anchor elements with an href attribute matching the regex */
    traverse(output->root, [&links, &champion_regex](const GumboNode* node) {
        if (node->v.element.tag == GUMBO_TAG_A) {
            auto href_attr = gumbo_get_attribute(&node->v.element.attributes, "href");
            boost::match_results<const char*> champion_match;
            if ((href_attr != nullptr) &&
                boost::regex_search(href_attr->value, champion_match, champion_regex)) {
                links.emplace(champion_match[1], champion_match[2], href_attr->value);
            }
        }
    });

    return links;
}

ChampionBuilds findBuilds(const std::string& html) {
    auto output = gumboParse(html.c_str(), html.size());
    auto wrappers = findBuildWrappers(output->root);

    ChampionBuilds builds;
    size_t i = 0;

    /* Map the wrappers to their wrapped builds */
    for (const auto& wrapper : wrappers) {
        builds.emplace_back(wrapper->prev->v.text.text, findItems(wrapper));
    }
    /* Some builds are not available on the website. Remove those with 0 items  */
    builds.erase(
        boost::remove_if(builds, [](const Build& build) { return build.second.size() == 0; }),
        std::end(builds));
    /* Emit a warning if we didn't find 4 builds */
    if (builds.size() != ExpectedNumBuilds) {
        std::cout << Color::Yellow
                  << boost::format(strings::expectedBuildsWarning) % builds.size() %
                         ExpectedNumBuilds << std::endl;
    }
    /* Remove duplicated builds */
    for (i = 0; i < builds.size(); ++i) {
        size_t to_keep = 0;
        auto match = std::find_if(std::begin(builds) + static_cast<std::ptrdiff_t>(i) + 1,
                                  std::end(builds),
                                  [&builds, &i, &to_keep](const Build& build) {
            /* Compare the two item vectors */
            if (build.second == builds[i].second) {
                to_keep = i;
                return true;
            }
            return false;
        });
        if (match != std::end(builds)) {
            /* Rename the build we keep */
            size_t index = builds[to_keep].first.find("%");
            if (index == std::string::npos) {
                /* "Most frequent ..." */
                builds[to_keep].first = builds[to_keep].first.substr(14);
            } else {
                /* "Highest win % ..." */
                builds[to_keep].first = builds[to_keep].first.substr(index + 2);
            }
            builds.erase(match);
        }
    }
    /* champion.gg does not track trinkets */
    for (auto&& build : builds) {
        /* If the build has machete, add a sweeper, otherwise add a totem */
        auto match =
            boost::find_if(build.second, [](const Item& item) { return item.first == "1039"; });
        if (match != std::end(build.second) && build.second.size() < 5) {
            build.second.emplace_back("3341", 1);
        } else {
            build.second.emplace_back("3340", 1);
        }
    }

    return builds;
}
