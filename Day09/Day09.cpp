#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

unsigned int getBasinSize(const std::pair<size_t, size_t> pos,
    const std::vector<std::string>& matrix, 
    std::vector<std::pair<size_t, size_t>>& visited)
{
    unsigned int rv = 1;
    if (pos.first != 0 &&
        matrix[pos.first - 1][pos.second] != '9' &&
        std::find(visited.cbegin(), visited.cend(), std::make_pair(pos.first - 1, pos.second)) == visited.cend()) {

        visited.emplace_back(pos.first - 1, pos.second);
        rv += getBasinSize(visited.back(), matrix, visited);
    }
    if (pos.first != matrix.size() - 1 &&
        matrix[pos.first + 1][pos.second] != '9' &&
        std::find(visited.cbegin(), visited.cend(), std::make_pair(pos.first + 1, pos.second)) == visited.cend()) {

        visited.emplace_back(pos.first + 1, pos.second);
        rv += getBasinSize(visited.back(), matrix, visited);
    }
    if (pos.second != 0 &&
        matrix[pos.first][pos.second - 1] != '9' &&
        std::find(visited.cbegin(), visited.cend(), std::make_pair(pos.first, pos.second - 1)) == visited.cend()) {

        visited.emplace_back(pos.first, pos.second - 1);
        rv += getBasinSize(visited.back(), matrix, visited);
    }
    if (pos.second != matrix.front().size() - 1 &&
        matrix[pos.first][pos.second + 1] != '9' &&
        std::find(visited.cbegin(), visited.cend(), std::make_pair(pos.first, pos.second + 1)) == visited.cend()) {

        visited.emplace_back(pos.first, pos.second + 1);
        rv += getBasinSize(visited.back(), matrix, visited);
    }
    return rv;
}

int main()
{
    std::ifstream file("input.txt");
    std::vector<std::string> matrix;
    for (std::string line; file >> line;) {
        matrix.emplace_back(std::move(line));
    }
    unsigned int riskLevel = 0;
    std::array<size_t,3> largestBasins{ 0, 0, 0};
    for (size_t l = 0; l < matrix.size(); ++l) {
        for (size_t c = 0; c < matrix[l].size(); ++c) {
            if (l != 0 && matrix[l][c] >= matrix[l - 1][c]) {
                continue;
            }
            if (l != matrix.size() - 1 && matrix[l][c] >= matrix[l + 1][c]) {
                continue;
            }
            if (c != 0 && matrix[l][c] >= matrix[l][c - 1]) {
                continue;
            }
            if (c != matrix[l].size() - 1 && matrix[l][c] >= matrix[l][c + 1]) {
                continue;
            }
            riskLevel += 1 + matrix[l][c] - '0';
            std::vector<std::pair<size_t, size_t>> visited{ {l, c} };
            const size_t basinSize = getBasinSize({ l, c }, matrix, visited);
            auto min = std::min_element(largestBasins.begin(), largestBasins.end());
            if (basinSize > *min) {
                *min = basinSize;
            }
        }
    }

    std::cout << "(Part 1) Risk level: " << riskLevel << std::endl;
    std::cout << "(Part 2) Largest basins: " << largestBasins[0] * largestBasins[1] * largestBasins[2];
        
    std::cin.get();
    return 0;
}
