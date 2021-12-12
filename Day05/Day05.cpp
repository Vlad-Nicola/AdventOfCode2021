#include <iostream>
#include <fstream>
#include <vector>

int main()
{
    std::ifstream file("input.txt");
    std::vector<std::vector<std::pair<int, int>>> grid(1000, std::vector<std::pair<int, int>>(1000, { 0, 0 }));
    for (int x1, y1, x2, y2; 
        file >> x1 && file.ignore() && file >> y1 && file.ignore(4) && file >> x2 && file.ignore() >> y2;){
        if (x1 == x2) {
            for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
                ++grid[x1][y].first; ++grid[x1][y].second;
            }
        } else if (y1 == y2) {
            for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
                ++grid[x][y1].first; ++grid[x][y1].second;
            }
        } else {
            const bool fY = y1 < y2 ? true : false;
            for (int i = 0, y = y1; fY && y <= y2 || !fY && y >= y2; y += fY ? 1 : -1, ++i) {
                ++grid[x1 < x2 ? x1 + i : x1 - i][y].second;
            }
        }
    }

    unsigned int totalResult_p1 = 0, totalResult_p2 = 0;
    for (unsigned int line = 0; line < grid.size(); ++line) {
        for (unsigned int col = 0; col < grid[line].size(); ++col) {
            totalResult_p1 += grid[line][col].first >= 2 ? 1 : 0;
            totalResult_p2 += grid[line][col].second >= 2 ? 1 : 0;
        }
    }

    std::cout << "(Part 1) Overlapping lines points: " << totalResult_p1 << std::endl;
    std::cout << "(Part 2) Overlapping lines points: " << totalResult_p2;

    return totalResult_p1 == 6666 && totalResult_p2 == 19081 ? 0 : -1;
}