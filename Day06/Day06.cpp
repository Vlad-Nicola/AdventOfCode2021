#include <iostream>
#include <fstream>
#include <vector>

uint64_t offspring(std::vector<uint64_t>& values, const short days)
{
    uint64_t res = 0;
    const short spawnedCount = (days + 6) / 7;
    for (short i = 0; i < spawnedCount; ++i) {
        const short newDay = days - 9 - (7 * i);
        if (newDay <= 0) {
            ++res;
            continue;
        }
        auto& v = values[newDay];
        if (v == 0) {
            v = offspring(values, newDay);
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
    for (unsigned short newFish; file >> newFish; file.ignore()) {
        offspringAfter256 += offspring(values, 256 - newFish);
        offspringAfter80 += offspring(values, 80 - newFish);
    }

    std::cout << "(Part 1) Fish after 80 days: " << offspringAfter80 << std::endl;
    std::cout << "(Part 2) Fish after 256 days: " << offspringAfter256;

    return offspringAfter80 == 379114 && offspringAfter256 == 1702631502303 ? 0 : -1;
}