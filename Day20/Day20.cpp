#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void expand(std::vector<std::string>& picture, const char c) {
    for (auto& line : picture) {
        line.insert(line.begin(), c);
        line.push_back(c);
    }
    picture.insert(picture.begin(), std::string(picture.front().size(), c));
    picture.insert(picture.end(), std::string(picture.front().size(), c));
}

int main()
{
    std::ifstream file("input.txt");
    constexpr unsigned int steps = 50;
    std::vector<std::string> picture;
    std::string function;
    std::getline(file, function);
    file.ignore();
    for (std::string line; file >> line; ) {
        picture.emplace_back("." + line + ".");
    }
    picture.insert(picture.begin(), 1, std::string(picture.front().size(), '.'));
    picture.insert(picture.end(), 1, std::string(picture.front().size(), '.'));

    unsigned int totalLit = 0, totalLit2 = 0;
    auto newPicture = picture;
    for (unsigned int step = 1; step <= steps; ++step) {
        unsigned int currentLit = 0;
        for (int i = 0; i < picture.size(); ++i) {
            for (int j = 0; j < picture.front().size(); ++j) {
                unsigned int pos = 0;
                for (int ii = -1; ii <= 1; ++ii) {
                    for (int jj = -1; jj <= 1; ++jj) {
                        if (i + ii < 0 || i + ii == picture.size() || j + jj < 0 || j + jj == picture.front().size())
                        {
                            if (step % 2 == 0) {
                                pos += 1 << (8 - (ii + 1) * 3 - (jj + 1));
                            }
                            continue;
                        }
                        else if (picture[i + ii][j + jj] == '#') {
                            pos += 1 << (8 - (ii + 1) * 3 - (jj + 1));
                        }
                    }
                }
                newPicture[i][j] = function[pos];
                if (function[pos] == '#') {
                    ++currentLit;
                }
            }
        }
        if (step != steps) {
            expand(newPicture, step % 2 ? '#' : '.');
        }
        if (step == 2) {
            totalLit2 = currentLit;
        }
        picture = newPicture;
        totalLit = currentLit;
    }
    
    std::cout << "(Part 1) Lit after 2 steps: " << totalLit2 << std::endl;
    std::cout << "(Part 2) Lit after 50 steps: " << totalLit;
    
    return totalLit2 == 5065 && totalLit == 14790 ? 0 : -1;
}
