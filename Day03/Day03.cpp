#include <iostream>
#include <fstream>
#include <array>
#include <bitset>
#include <vector>
#include <algorithm>

int main()
{
    constexpr unsigned int sz = 12;
    std::ifstream file("input.txt");
    std::array<int, sz> counter{0};
    std::vector<std::tuple<std::bitset<sz>, bool, bool>> numbers;
    std::bitset<sz> currentNumber;
    for (unsigned char c, pos = 0; file >> c; pos = (pos + 1) % sz) {
        counter[pos] += c == '1' ? 1 : -1;
        currentNumber[sz - pos - 1] = c - '0';
        if (pos == sz - 1) {
            numbers.emplace_back(currentNumber, true, true);
        }
    }

    for (unsigned int i = 0; i < sz; ++i) {
        currentNumber[sz - i - 1] = counter[i] < 0 ? 0 : 1;
        unsigned int size_o2 = 0, size_o2_1 = 0, size_co2 = 0, size_co2_1 = 0;
        for (const auto& [number, valid_o2, valid_co2] : numbers) {
            size_o2 += valid_o2 ? 1 : 0;
            size_o2_1 += (valid_o2 && number[sz - i - 1] == 1) ? 1 : 0;
            size_co2 += valid_co2 ? 1 : 0;
            size_co2_1 += (valid_co2 && number[sz - i - 1] == 1) ? 1 : 0;
        }
        for (auto& [number, valid_o2, valid_co2] : numbers) {
            if (size_o2 > 1 && valid_o2 && (number[sz - i - 1] != (size_o2_1 >= (size_o2 + 1) / 2) ? 1 : 0)) {
                valid_o2 = false;
            }
            if (size_co2 > 1 && valid_co2 && (number[sz - i - 1] != (size_co2_1 >= (size_co2 + 1) / 2) ? 0 : 1)) {
                valid_co2 = false;
            }
        }
    }

    unsigned long nr_o2 = 0, nr_co2 = 0;
    for (const auto& [number, valid_o2, valid_co2] : numbers) {
        if (valid_o2) {
            nr_o2 = number.to_ulong();
        }
        if (valid_co2) {
            nr_co2 = number.to_ulong();
        }
    }

    unsigned long fuel = currentNumber.to_ulong() * (~currentNumber).to_ulong(), scrubber = nr_o2 * nr_co2;
    std::cout << "(Part 1) Fuel consumption: " << fuel << std::endl;
    std::cout << "(Part 2) CO2 scrubber rating: " << scrubber;

    return fuel == 3309596 && scrubber == 2981085 ? 0 : -1;
}