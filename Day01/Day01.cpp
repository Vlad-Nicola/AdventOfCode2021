#include <iostream>
#include <fstream>
#include <list>
#include <numeric>

unsigned int doMeasurement(const unsigned int slidingWindow)
{
    std::ifstream file("input.txt");
    unsigned int total = 0;
    std::list<unsigned int> window;

    //Initialize window
    for (unsigned int val, i = 0; i < slidingWindow && file >> val; ++i) {
        window.emplace_back(val);
    }

    //Continue sliding the window and incrementing the counter if needed
    unsigned int windowSize = std::accumulate(window.cbegin(), window.cend(), 0);
    for (unsigned int val; file >> val; ) {
        const unsigned int newWindowSize = windowSize + val - window.front();
        if (windowSize < newWindowSize) {
            ++total;
        }
        windowSize = newWindowSize;
        window.pop_front();
        window.emplace_back(val);
    }

    return total;
}

int main()
{
    std::cout << "(Part 1) Increased count: " << doMeasurement(1) << std::endl;
    std::cout << "(Part 2) Increased count: " << doMeasurement(3);

    std::cin.get();
    return 0;
}