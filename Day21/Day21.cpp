#include <iostream>
#include <fstream>
#include <vector>
#include <array>

uint64_t getNext(uint64_t& die) {
    uint64_t rv = 0;
    for (unsigned int i = 0; i < 3; ++i) {
        ++die;
        die = (die - 1) % 100 + 1;
        rv += die;
    }
    return rv;
}

uint64_t deterministicGame(const int pos1, const int pos2) {
    std::pair<int, int> p1{ pos1, 0 }, p2{ pos2, 0 };
    uint64_t die = 0, rollCount = 0, loserScore = 0;
    while (true) {
        uint64_t roll = getNext(die);
        rollCount += 3;
        p1.first = (p1.first + roll - 1) % 10 + 1;
        p1.second += p1.first;
        if (p1.second >= 1000) {
            loserScore = p2.second;
            break;
        }

        roll = getNext(die);
        rollCount += 3;
        p2.first = (p2.first + roll - 1) % 10 + 1;
        p2.second += p2.first;
        if (p2.second >= 1000) {
            loserScore = p1.second;
            break;
        }
    }
    return rollCount * loserScore;
}

void playGame(uint64_t* res, std::array<std::pair<int, int>, 2> p, const int ord, uint64_t multiplyer) {
    static constexpr std::array<short, 7> multiMap{ 1, 3, 6, 7, 6, 3, 1 };
    if (p[1 - ord].second >= 21) {
        res[1 - ord] += multiplyer;
        return;
    }
    auto oldp = p[ord];
    for (int i = 3; i <= 9; ++i) {
        p[ord].first = (oldp.first + i - 1) % 10 + 1;
        p[ord].second = oldp.second + p[ord].first;
        playGame(res, p, 1 - ord, multiplyer * multiMap[i - 3]);
    }
}

int main()
{
    std::ifstream file("input.txt");
    int pos1, pos2;
    file.ignore(28);
    file >> pos1;
    file.ignore(28);
    file >> pos2;

    const uint64_t res1 = deterministicGame(pos1, pos2);
    uint64_t res2[2] = { 0, 0 };
    std::array<std::pair<int, int>, 2> p = { std::make_pair(pos1, 0), std::make_pair(pos2, 0) };
    playGame(res2, p, 0, 1);

    std::cout << "(Part 1) First game: " << res1 << std::endl;
    std::cout << "(Part 2) Second game: " << std::max(res2[0], res2[1]);

    return res1 == 1196172 && std::max(res2[0],res2[1]) == 106768284484217 ? 0 : -1;
}
