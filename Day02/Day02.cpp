#include <iostream>
#include <fstream>
#include <list>
#include <numeric>

int main()
{
    std::ifstream file("input.txt");
    int aim = 0;
    unsigned int distance = 0, realDepth = 0;
    for (std::string dir; file >> dir;) {
        unsigned int amount;
        file >> amount;
        if (!dir.compare("forward")) {
            distance += amount;
            realDepth += amount * aim;
        } else {
            aim += dir.compare("up") ? amount : -amount;
        }
    }

    std::cout << "(Part 1) Direction: " << distance * aim << std::endl;
    std::cout << "(Part 2) Real Direction: " << distance * realDepth;

    std::cin.get();
    return 0;
}