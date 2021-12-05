#include <iostream>
#include <fstream>
#include <vector>

int main()
{
    std::ifstream file("input.txt");
    std::vector<std::vector<unsigned int>> grid_p1(1000, std::vector<unsigned int>(1000, 0)), grid_p2 = grid_p1;
    for (int x1, y1, x2, y2; 
        file >> x1 && file.ignore() && file >> y1 && file.ignore(4) && file >> x2 && file.ignore() >> y2;){
        if (x1 == x2) {
            for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
                ++grid_p1[x1][y]; ++grid_p2[x1][y];
            }
        } else if (y1 == y2) {
            for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
                ++grid_p1[x][y1]; ++grid_p2[x][y1];
            }
        } else {
            const bool fY = y1 < y2 ? true : false;
            for (int i = 0, y = y1; fY && y <= y2 || !fY && y >= y2; y += fY ? 1 : -1, ++i) {
                ++grid_p2[x1 < x2 ? x1 + i : x1 - i][y];
            }
        }
    }

    unsigned int totalResult_p1 = 0, totalResult_p2 = 0;
    for (unsigned int line = 0; line < grid_p1.size(); ++line) {
        for (unsigned int col = 0; col < grid_p1[line].size(); ++col) {
            totalResult_p1 += grid_p1[line][col] >= 2 ? 1 : 0;
            totalResult_p2 += grid_p2[line][col] >= 2 ? 1 : 0;
        }
    }

    std::cout << "(Part 1) Overlapping lines points: " << totalResult_p1 << std::endl;
    std::cout << "(Part 2) Overlapping lines points: " << totalResult_p2;

    std::cin.get();
    return 0;
}