#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

constexpr unsigned steps = 40, m = 26, im = m * steps, IM = im * m, steps_p1 = 10;

void addHist(std::vector<uint64_t>& histCache,
             const std::vector<uint8_t>& children,
             const uint8_t first,
             const uint8_t second,
             const uint8_t step,
             uint64_t* total)
{
    //If simple split, just add the value
    if (step == 1) {
        ++total[children[first * m + second]];
        return;
    }
    //Get this hist
    uint64_t* hist = &histCache[first * IM + second * im + (step - 1) * m];
    //See if cached
    if (*hist != 0 || memcmp(hist, hist + 1, (m - 1) * sizeof(uint64_t))) {
        for (unsigned int i = 0; i < m; ++i) {
            total[i] += hist[i];
        }
        return;
    }
    //Not cached, split
    auto child = children[first * m + second];
    uint64_t totalLeft[m]{ 0 }, totalRight[m]{ 0 };
    addHist(histCache, children, first, child, step - 1, totalLeft);
    addHist(histCache, children, child, second, step - 1, totalRight);
    //Collect result and cache
    ++hist[child];
    for (unsigned int i = 0; i < m; ++i) {
        hist[i] += totalRight[i] + totalLeft[i];
        total[i] += hist[i];
    }
}

int main()
{
    std::ifstream file("input.txt");
    std::vector<uint64_t> cache(IM * m, 0);
    std::vector<uint8_t> children(m * m);
    std::string pattern;
    std::getline(file, pattern).ignore();
    for (std::string line; std::getline(file, line);) {
        children[(line[0] - 'A') * m + (line[1] - 'A')] = line.back() - 'A';
    }

    uint64_t result[m]{ 0 }, result_p1[m]{ 0 };
    for (unsigned int i = 1; i < pattern.size(); ++i) {
        addHist(cache, children, pattern[i - 1] - 'A', pattern[i] - 'A', steps, result);
        addHist(cache, children, pattern[i - 1] - 'A', pattern[i] - 'A', steps_p1, result_p1);
        ++result[pattern[i - 1] - 'A'];
        ++result_p1[pattern[i - 1] - 'A'];
    }
    ++result[pattern.back() - 'A'];
    ++result_p1[pattern.back() - 'A'];

    uint64_t min = -1, max = 0, min_p1 = -1, max_p1 = 0;
    for (unsigned int i = 0; i < m; ++i) {
        if (result[i] != 0) {
            if (result[i] < min) {
                min = result[i];
            }
            if (result[i] > max) {
                max = result[i];
            }
        }
        if (result_p1[i] != 0) {
            if (result_p1[i] < min_p1) {
                min_p1 = result_p1[i];
            }
            if (result_p1[i] > max_p1) {
                max_p1 = result_p1[i];
            }
        }
    }

    std::cout << "(Part 1) After 10 steps: " << max_p1 - min_p1 << std::endl;
    std::cout << "(Part 2) After 40 steps: " << max - min;
    
    return max_p1 - min_p1 == 3306 && max - min == 3760312702877 ? 0 : -1;
}
