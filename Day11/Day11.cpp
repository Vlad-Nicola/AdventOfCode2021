#include <iostream>
#include <fstream>
#include <array>

using OctoType = std::array<std::array<unsigned char, 10>, 10>;
unsigned int flashNeighb(const std::pair<unsigned char, unsigned char>& pos, OctoType& octo);

unsigned int flashOcto(const std::pair<unsigned char, unsigned char>& pos, OctoType& octo)
{
    if (octo[pos.first][pos.second] > '9') {
        octo[pos.first][pos.second] = '0';
        return 1 + flashNeighb(pos, octo);
    }
    return 0;
}

unsigned int flashNeighb(const std::pair<unsigned char, unsigned char>& pos, OctoType& octo)
{
    unsigned int totalFlashes = 0;
    for (int i = std::max(pos.first - 1, 0); i <= std::min(pos.first + 1, 9); ++i) {
        for (int j = std::max(pos.second - 1, 0); j <= std::min(pos.second + 1, 9); ++j) {
            if (i == pos.first && j == pos.second) {
                continue;
            }
            if (octo[i][j] != '0') {
                ++octo[i][j];
                totalFlashes += flashOcto({i,j}, octo);
            }
        }
    }
    return totalFlashes;
}

int main()
{
    std::ifstream file("input.txt");
    OctoType octo;
    for (unsigned char c, i = 0; file >> c; ++i) {
        octo[i / 10][i % 10] = c;
    }

    unsigned int totalFlashes = 0, magicStep = 0;
    constexpr unsigned int steps = 100;
    for (unsigned int step = 1; ; ++step) {
        unsigned int stepFlashes = 0;
        for (unsigned int i = 0; i < 10; ++i) {
            for (unsigned int j = 0; j < 10; ++j) {
                ++octo[i][j];
            }
        }
        for (unsigned int i = 0; i < 10; ++i) {
            for (unsigned int j = 0; j < 10; ++j) {
                stepFlashes += flashOcto({ i, j }, octo);
            }
        }
        if(stepFlashes == steps) {
            magicStep = step;
        }
        if (step <= steps) {
            totalFlashes += stepFlashes;
        } else if (magicStep != 0) {
            break;
        }
    }

    std::cout << "(Part 1) Total flashes: " << totalFlashes << std::endl;
    std::cout << "(Part 2) Step where all flash: " << magicStep;

    std::cin.get();
    return 0;
}
