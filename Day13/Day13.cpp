#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main()
{
    std::ifstream file("input.txt");
    std::vector<std::vector<bool>> paper;
    uint64_t sizeX = 0, sizeY = 0;
    for (int x, y; file >> y && file.ignore() && file >> x && file.ignore();) {
        const int difX = x - (int)paper.size() + 1;
        if (difX > 0) {
            paper.insert(paper.cend(), difX, std::vector<bool>(paper.empty() ? 0 : paper.front().size(), false));
            sizeX = x + 1;
        }
        const int difY = y - (int)paper.front().size() + 1;
        if (difY > 0) {
            for (auto& line : paper) {
                line.insert(line.cend(), difY, false);
            }
            sizeY = y + 1;
        }
        paper[x][y] = true;

        if (file.peek() == '\n') {
            break;
        }
    }
    unsigned int firstCount = 0;
    for (std::string line; std::getline(file, line, '='); ) {
        uint64_t val;
        file >> val;
        unsigned int count = 0;
        if (line.back() == 'y') {
            for (unsigned int i = 1; i < val + 1 ; ++i) {
                for (unsigned int j = 0; j < sizeY; ++j) {
                    paper[val - i][j] = paper[val - i][j] || paper[val + i][j];
                    if (paper[val - i][j]) {
                        ++count;
                    }
                }
            }
            sizeX -= val + 1;
        } else {
            for (unsigned int i = 0; i < sizeX; ++i) {
                for (unsigned int j = 1; j < val + 1; ++j) {
                    paper[i][val - j] = paper[i][val - j] || paper[i][val + j];
                    if (paper[i][val - j]) {
                        ++count;
                    }
                }
            }
            sizeY -= val + 1;
        }
        if (0 == firstCount) {
            firstCount = count;
        }
    }

    std::cout << "(Part 1) Visible dots first step: " << firstCount << std::endl;
    std::cout << "(Part 2) Code:" << std::endl << std::endl;

    for (unsigned int i = 0; i < sizeX; ++i) {
        for (unsigned int j = 0; j < sizeY; ++j) {
            if (j % 5 == 0) {
                std::cout << " ";
            }
            std::cout << (paper[i][j] ? "#" : " ");
        }
        std::cout << std::endl;
    }

    return firstCount == 790 ? 0 : -1;
}
