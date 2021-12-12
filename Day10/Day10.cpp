#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

constexpr char getPairChar(const char c) {
    switch (c) {
    case ')': return '(';
    case ']': return '[';
    case '}': return '{';
    default: return '<';
    }
}

constexpr bool isOpenChar(const char c) {
    return c == '(' || c == '[' || c == '{' || c == '<';
}

static const std::unordered_map<char, const unsigned int> syntaxScoreTable{ {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };
static const std::unordered_map<char, const unsigned int> autocompleteScoreTable{ {'(', 1}, {'[', 2}, {'{', 3}, {'<', 4} };

bool parseLine(const std::string& line, uint64_t& res)
{
    std::vector<char> stack;
    for (const char c : line) {
        if (isOpenChar(c)) {
            stack.push_back(c);
        } else {
            if (getPairChar(c) != stack.back()) {
                res = syntaxScoreTable.find(c)->second;
                return false;
            }
            stack.pop_back();
        }
    }
    for (auto it = stack.crbegin(); it != stack.crend(); ++it) {
        res = res * 5 + autocompleteScoreTable.find(*it)->second;
    }
    return true;
}

int main()
{
    std::ifstream file("input.txt");
    
    uint64_t syntaxScore = 0;
    std::vector<uint64_t> autocompleted;
    for (std::string line; file >> line; ) {
        uint64_t res = 0;
        if (parseLine(line, res)) {
            autocompleted.push_back(res);
        } else {
            syntaxScore += res;
        }
    }
    std::nth_element(autocompleted.begin(), autocompleted.begin() + autocompleted.size() / 2, autocompleted.end());

    std::cout << "(Part 1) Total score for syntax errors: " << syntaxScore << std::endl;
    std::cout << "(Part 2) Total score for autocomplete: " << autocompleted[autocompleted.size() / 2];

    std::cin.get();
    return 0;
}
