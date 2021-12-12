#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main()
{
    std::ifstream file("input.txt");
    std::vector<unsigned short> crabs;
    for (unsigned short val; file >> val; file.ignore()) {
        crabs.push_back(val);
    }
    std::sort(crabs.begin(), crabs.end());
    unsigned int minDirectFuel = std::numeric_limits<unsigned int>::max();
    unsigned int minAccelFuel = std::numeric_limits<unsigned int>::max();
    for (int i = crabs.front(); i != crabs.back(); ++i) {
        unsigned int directFuelConsumption = 0, accelFuelConsumption = 0;
        for (const auto& crab : crabs) {
            const unsigned int diff = std::abs(i - crab);
            directFuelConsumption += diff;
            accelFuelConsumption += diff * (diff + 1) / 2;
        }
        if (minDirectFuel > directFuelConsumption) {
            minDirectFuel = directFuelConsumption;
        } 
        if (minAccelFuel > accelFuelConsumption) {
            minAccelFuel = accelFuelConsumption;
        }
    }

    std::cout << "(Part 1) Min direct fuel consumption: " << minDirectFuel << std::endl;
    std::cout << "(Part 2) Min accelerated fuel consumption: " << minAccelFuel;

    return minDirectFuel == 344138 && minAccelFuel == 94862124 ? 0 : -1;
}