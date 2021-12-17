#include <iostream>
#include <fstream>

int shoot(std::pair<int, int>& pos, 
    std::pair<int, int>& vel,
    const std::pair<int, int>& targetX,
    const std::pair<int, int>& targetY)
{
    pos.first += vel.first;
    pos.second += vel.second;
    vel.first += vel.first < 0 ? 1 : vel.first > 0 ? -1 : 0;
    vel.second -= 1;

    if (pos.first >= targetX.first &&
        pos.first <= targetX.second &&
        pos.second >= targetY.first &&
        pos.second <= targetY.second)
    {
        return 1;
    }

    if (pos.first > targetX.second || pos.second < targetY.first) {
        return 0;
    }

    return shoot(pos, vel, targetX, targetY);
}

int main()
{
    std::ifstream file("input.txt");
    std::pair<int, int> targetX, targetY;
    file.ignore(15);
    file >> targetX.first;
    file.ignore(2);
    file >> targetX.second;
    file.ignore(4);
    file >> targetY.first;
    file.ignore(2);
    file >> targetY.second;

    const int maxHeight = targetY.first * (targetY.first + 1) / 2;
    const int maxVelY = -1 - targetY.first;
    const int minVelY = targetY.first;
    const int maxVelX = targetX.second;
    int minVelX = 0, val = 0;
    for (int i = 1; ; ++i) {
        val += i;
        if (val >= targetX.first) {
            minVelX = i - 1;
            break;
        }
    }

    unsigned int totalOptions = 0;
    for (int velX = minVelX; velX <= maxVelX; ++velX) {
        for (int velY = minVelY; velY <= maxVelY; ++velY) {
            std::pair<int, int> pos{ 0,0 }, vel{ velX, velY };
            totalOptions += shoot(pos, vel, targetX, targetY);
        }
    }

    std::cout << "(Part 1) Maximum height: " << maxHeight << std::endl;
    std::cout << "(Part 2) Total options: " << totalOptions;

    return maxHeight == 12246 && totalOptions == 3528 ? 0 : -1;
}
