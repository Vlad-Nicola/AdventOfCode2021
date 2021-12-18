#include <iostream>
#include <fstream>
#include <vector>

struct SN
{
    SN() = default;
    SN(const SN* other, SN* parent = nullptr) : parent(parent){
        if (other->left) {
            left = new SN(other->left, this);
            right = new SN(other->right, this);
        } else {
            reg = other->reg;
        }
    }
    SN(const int reg, SN* parent = nullptr) : reg(reg), parent(parent) {}
    SN(const std::string& str, SN* parent = nullptr) : parent(parent) {
        if (str.size() == 1) {
            reg = str.front() - '0';
            return;
        }

        unsigned brackets = 0;
        for (unsigned i = 1; i < str.size(); ++i) {
            switch (str[i]) {
            case ',':
                if (!brackets) {
                    left = new SN(str.substr(1, i - 1), this);
                    right = new SN(str.substr(i + 1, str.size() - i - 2), this);
                    return;
                }
                break;
            case '[': ++brackets; break;
            case ']': --brackets;
            }
        }
    }

    SN& operator+=(SN* other) {
        SN* newLeft = new SN();
        left->parent = newLeft;
        right->parent = newLeft;
        newLeft->left = left;
        newLeft->right = right;
        newLeft->parent = this;
        left = newLeft;
        other->parent = this;
        right = other;

        while (explode() || split());

        return *this;
    }

    bool split() {
        if (!left) {
            if (reg >= 10) {
                left = new SN(reg / 2, this);
                right = new SN((reg + 1) / 2, this);
                reg = -1;
                return true;
            } else {
                return false;
            }
        }
        return left->split() || right->split();
    }

    bool explode(const int depth = 0) {
        if (depth == 4) {
            if (left && right)
            {
                const int valLeft = left->reg, valRight = right->reg;
                SN* p = parent, *c = this;
                while (p && p->left == c) {
                    c = p;
                    p = p->parent;
                }
                if (p) {
                    c = p->left;
                    while (c) {
                        p = c;
                        c = c->right;
                    }
                    p->reg += valLeft;
                }
                p = parent, c = this;
                while (p && p->right == c) {
                    c = p;
                    p = p->parent;
                }
                if (p) {
                    c = p->right;
                    while (c) {
                        p = c;
                        c = c->left;
                    }
                    p->reg += valRight;
                }

                reg = 0;
                delete left; left = nullptr;
                delete right; right = nullptr;
                return true;
            } else {
                return false;
            }
        } 
        const int newDepth = depth + 1;
        return left && left->explode(newDepth) || right && right->explode(newDepth);
    }

    int getSumMagnitude(const SN* other) const {
        SN* newNumber(new SN(this));
        SN* newOther(new SN(other));
        *newNumber += newOther;

        return newNumber->getMagnitude();
    }

    int getMagnitude() const {
        return left ? 3 * left->getMagnitude() + 2 * right->getMagnitude() : reg;
    }

    SN *parent = nullptr, *left = nullptr, *right = nullptr;
    int reg = -1;
};

int main()
{
    std::ifstream file("input.txt");
    std::vector<SN*> numbers;
    for (std::string line; file >> line; ) {
        numbers.emplace_back(new SN(line));
    }

    int maxMagn = 0;
    for (auto itf = numbers.cbegin(); itf != numbers.cend(); ++itf) {
        for (auto its = numbers.cbegin(); its != numbers.cend(); ++its) {
            if (itf == its) {
                continue;
            }
            const int sumMagn = (*itf)->getSumMagnitude(*its);
            if (sumMagn > maxMagn) {
                maxMagn = sumMagn;
            }
        }
    }

    for (unsigned int i = 1; i < numbers.size(); ++i) {
        numbers[0]->operator+=(numbers[i]);
    }
    const int totalMagnitude = numbers[0]->getMagnitude();

    std::cout << "(Part 1) Magnitude: " << totalMagnitude << std::endl;
    std::cout << "(Part 2) Maximum sum magnitude: " << maxMagn;

    return totalMagnitude == 4480 && maxMagn == 4676 ? 0 : -1;
}
