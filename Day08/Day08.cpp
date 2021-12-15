#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>

struct Segment
{
    std::vector<std::string> hints;
    std::string one, four, seven;

    void createDisplayMapping() {
        for (const auto& hint : hints) {
            const size_t sz = hint.size();
            switch (sz) {
            case 2:
                one = hint;
                break;
            case 3:
                seven = hint;
                break;
            case 4:
                four = hint;
            }
        }
    }

    char getMissingLetter(const std::string& digit) const {
        for (unsigned int i = 0; i < 7; ++i) {
            if (digit.find('a' + i) == std::string::npos) {
                return 'a' + i;
            }
        }
        return 'a';
    }

    std::pair<char, char> get2MissingLetters(const std::string& digit) const {
        std::pair rv{ -1, -1 };
        for (unsigned int i = 0; i < 7; ++i) {
            if (digit.find('a' + i) == std::string::npos) {
                if (rv.first != -1) {
                    rv.second = 'a' + i;
                    return rv;
                }
                rv.first = 'a' + i;
            }
        }
    }

    std::pair<unsigned, unsigned> getOutputs() const {
        std::pair<unsigned, unsigned> rv = { 0, 0 };
        for (size_t i = hints.size() - 4; i < hints.size(); ++i) {
            const unsigned int pos = std::pow(10, hints.size() - i - 1);
            const size_t sz = hints[i].size();
            switch (sz) {
            case 2:
                rv.first += pos;
                ++rv.second;
                break;
            case 3:
                rv.first += pos * 7;
                ++rv.second;
                break;
            case 4:
                rv.first += pos * 4;
                ++rv.second;
                break;
            case 7:
                rv.first += pos * 8;
                ++rv.second;
                break;
            case 6: {
                const char c = getMissingLetter(hints[i]);
                if (one.size()) {
                    if (one.find(c) != std::string::npos) {
                        rv.first += pos * 6;
                        break;
                    }
                }
                if (seven.size()) {
                    if (seven.find(c) != std::string::npos) {
                        rv.first += pos * 6;
                        break;
                    }
                }
                if (four.size()) {
                    if (four.find(c) != std::string::npos) {
                        rv.first += pos * 0;
                        break;
                    }
                }
                rv.first += pos * 9;
                break;
            }
            case 5:
                const auto p = get2MissingLetters(hints[i]);
                if (one.size()) {
                    if (one.find(p.first) == std::string::npos && one.find(p.second) == std::string::npos) {
                        rv.first += pos * 3;
                        break;
                    }
                }
                if (seven.size()) {
                    if (seven.find(p.first) == std::string::npos && seven.find(p.second) == std::string::npos) {
                        rv.first += pos * 3;
                        break;
                    }
                }
                if (four.size()) {
                    if (four.find(p.first) != std::string::npos && four.find(p.second) != std::string::npos) {
                        rv.first += pos * 2;
                        break;
                    }
                }
                rv.first += pos * 5;
            }
        }
        return rv;
    }
};

int main()
{
    std::ifstream file("input.txt");
    std::vector<Segment> segments;
    Segment newSegment;
    for (std::string word; file >> word; ) {
        if (word.compare("|")) {
            newSegment.hints.emplace_back(std::move(word));
        } else {
            for (unsigned int i = 0; i < 4 && file >> word; ++i) {
                newSegment.hints.emplace_back(std::move(word));
            }
            segments.emplace_back(std::move(newSegment));
        }
    }

    std::pair<unsigned, unsigned> total = { 0,0 };
    for (auto& segment : segments) {
        segment.createDisplayMapping();
        const auto val = segment.getOutputs();
        total.first += val.first;
        total.second += val.second;
    }

    std::cout << "(Part 1) Output digits with unique number of segments: " << total.second << std::endl;
    std::cout << "(Part 2) Total: " << total.first << std::endl;

    return total.second == 247 && total.first == 933305 ? 0 : -1;
}
