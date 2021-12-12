#include <iostream>
#include <fstream>
#include <array>
#include <bitset>
#include <vector>
#include <algorithm>

class Board
{
public:
    typedef std::pair<unsigned int, bool> ElemType;
    typedef std::vector<ElemType> LineType;
    typedef std::vector<LineType> BoardType;

    Board(const BoardType& board) : board(board) {}

    void mark(const unsigned int val) {
        for (auto& line : board) {
            for (auto& [column, hit] : line) {
                if (column == val) {
                    hit = true;
                }
            }
        }
    }

    bool isWinner(unsigned int& val) const {
        bool winner = false;
        std::array<bool, 5> columnWinner{ true, true, true, true, true };
        for (auto& line : board) {
            bool rowWinner = true;
            for (unsigned int column = 0; column < 5; ++column) {
                auto& elem = line[column];
                if (!elem.second) {
                    rowWinner = false;
                    columnWinner[column] = false;
                    val += elem.first;
                }
            }
            if (rowWinner) {
                winner = true;
            }
        }

        return winner || std::any_of(columnWinner.cbegin(), columnWinner.cend(), [](const auto val) {return val; });
    }

private:
    BoardType board;
};

std::pair<unsigned int, unsigned int> 
playBingo(std::vector<Board>& boards, const std::vector<unsigned int>& numbers)
{
    std::pair<unsigned int, unsigned int> retVal;
    const size_t initialSize = boards.size();
    for (const auto number : numbers) {
        for (auto board = boards.begin(); board != boards.end();) {
            board->mark(number);
            unsigned int val = 0;
            if (board->isWinner(val)) {
                if (boards.size() == 1) {
                    retVal.second = number * val;
                    return retVal;
                } else if(boards.size() == initialSize) {
                    retVal.first = number * val;
                }
                board = boards.erase(board);
                continue;
            }
            ++board;
        }
    }
    return retVal;
}

int main()
{
    std::ifstream file("input.txt");
    std::vector<unsigned int> chosenNumbers;
    do {
        unsigned int number;
        file >> number;
        chosenNumbers.emplace_back(number);
    } while (file.get() == ',');

    std::vector<Board> boards;
    while (!file.eof()) {
        Board::BoardType newBoard;
        for (unsigned int i = 0; i < 5; ++i) {
            Board::LineType newLine;
            for (unsigned int j = 0; j < 5; ++j) {
                unsigned int elem;
                file >> elem;
                newLine.emplace_back(elem, false);
            }
            newBoard.emplace_back(std::move(newLine));
        }
        boards.emplace_back(std::move(newBoard));
    }

    const auto res = playBingo(boards, chosenNumbers);
    std::cout << "(Part 1) First Winner score: " << res.first << std::endl;
    std::cout << "(Part 1) Last Winner score: " << res.second;

    return res.first == 39984 && res.second == 8468 ? 0 : -1;
}