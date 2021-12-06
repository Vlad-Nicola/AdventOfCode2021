#include <iostream>
#include <fstream>
#include <vector>

uint64_t offspring(std::vector<uint64_t>& values, const short val, const short days)
{
    if (val >= days) {
        return 1;
    }

    uint64_t res = 0;
    const short spawnedCount = (days - val - 1) / 7 + 1;
    for (short i = 0; i < spawnedCount; ++i) {
        const short newDay = days - val - 3 - (7 * i);
        if (newDay <= 0) {
            ++res;
            continue;
        }
        auto& v = values[newDay];
        if (v == 0) {
            v = offspring(values, 6, newDay);
        }
        res += v;
    }
    return 1 + res;
}

int main()
{
    std::ifstream file("input.txt");

    std::vector<uint64_t> values(256, 0);
    uint64_t offspringAfter256 = 0, offspringAfter80 = 0;
    constexpr unsigned short days = 256;
    for (unsigned short newFish; file >> newFish; file.ignore()) {
        offspringAfter256 += offspring(values, newFish, 256);
        offspringAfter80 += offspring(values, newFish, 80);
    }

    std::cout << "(Part 1) Fish after 80 days: " << offspringAfter80 << std::endl;
    std::cout << "(Part 2) Fish after 256 days: " << offspringAfter256;

    std::cin.get();
    return 0;
}