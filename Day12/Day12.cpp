#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

std::pair<unsigned, unsigned> getPaths(
    const std::string& elem, 
    const std::unordered_map<std::string, std::vector<std::string>>& map, 
    std::vector<std::string> visited,
    bool allowSmallTwice)
{
    if (elem == "end") {
        return { 1,1 };
    }

    if (elem[0] > 'Z') {
        if (std::find(visited.cbegin(), visited.cend(), elem) != visited.cend()) {
            if (!allowSmallTwice || elem == "start") {
                return { 0,0 };
            }
            allowSmallTwice = false;
        } else {
            visited.push_back(elem);
        }
    }

    std::pair<unsigned, unsigned> paths = { 0,0 };
    for (const auto& neigh : map.at(elem)) {
        const auto res = getPaths(neigh, map, visited, allowSmallTwice);
        paths.second += res.second;
        if (allowSmallTwice) {
            paths.first += res.first;
        }
    }
    return paths;
}

int main()
{
    std::ifstream file("input.txt");
    std::unordered_map<std::string, std::vector<std::string>> map;
    for (std::string line; file >> line;) {
        const size_t pos = line.find('-');
        map[line.substr(0, pos)].emplace_back(line.substr(pos + 1));
        map[line.substr(pos + 1)].emplace_back(line.substr(0, pos));
    }

    const auto res = getPaths("start", map, {}, true);
    std::cout << "(Part 1) Visited paths count: " << res.first << std::endl;
    std::cout << "(Part 2) Visited paths count (one double allowed): " << res.second << std::endl;
    
    return res.first == 3679 && res.second == 107395 ? 0 : -1;
}
