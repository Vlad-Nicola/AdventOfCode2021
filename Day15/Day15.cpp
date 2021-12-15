#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

struct Node
{
    Node(const unsigned int cost, const std::pair<size_t, size_t>& pos) : cost(cost), pos(pos) {}
    Node() = default;
    Node* prev = nullptr;
    unsigned int cost = 0;
    std::pair<size_t, size_t> pos;
};

struct Cmp
{
    bool operator()(const Node* left, const Node* right) const
    {
        return right->cost < left->cost;
    }
};

bool doCheck(Node*& current, Node*& next)
{
    if (!next->prev) {
        next->cost += current->cost;
        next->prev = current;
        return true;
    }
    if (next->prev->cost > current->cost) {
        next->cost -= next->prev->cost - current->cost;
        next->prev = current;
    }
    return false;
}

void getShortest(std::vector<std::vector<Node>>& table)
{
    std::priority_queue<Node*, std::vector<Node*>, Cmp> queue(Cmp{});
    queue.push(&table[0][0]);
    while (queue.size()) {
        Node* current = queue.top();
        queue.pop();

        if (current->pos.first != 0) {
            Node* next = &table[current->pos.first - 1][current->pos.second];
            if (doCheck(current, next)) {
                queue.push(next);
            }
        }
        if (current->pos.first != table.size() - 1) {
            Node* next = &table[current->pos.first + 1][current->pos.second];
            if (doCheck(current, next)) {
                queue.push(next);
            }
        }
        if (current->pos.second != 0) {
            Node* next = &table[current->pos.first][current->pos.second - 1];
            if (doCheck(current, next)) {
                queue.push(next);
            }
        }
        if (current->pos.second != table.back().size() - 1) {
            Node* next = &table[current->pos.first][current->pos.second + 1];
            if (doCheck(current, next)) {
                queue.push(next);
            }
        }
    }
}

int main()
{
    std::ifstream file("input.txt");
    std::vector<std::vector<Node>> table;
    for (std::string line; file >> line;) {
        table.emplace_back();
        for (const auto c : line) {
            table.back().emplace_back(c - '0', std::make_pair(table.size() - 1, table.back().size()));
        }
    }

    std::vector<std::vector<Node>> bigTable(5 * table.size(), std::vector<Node>(5 * table.front().size()));
    for (unsigned int small_i = 0; small_i < table.size(); ++small_i) {
        for (unsigned int small_j = 0; small_j < table[small_i].size(); ++small_j) {
            for (unsigned int i = 0; i < 5; ++i) {
                for (unsigned int j = 0; j < 5; ++j) {
                    auto& tg = bigTable[small_i + i * table.size()][small_j + j * table[small_i].size()];
                    tg.cost = table[small_i][small_j].cost + j + i;
                    if (tg.cost > 9) {
                        tg.cost -= 9;
                    }
                    tg.pos = std::make_pair(small_i + i * table.size(), small_j + j * table[small_i].size());
                }
            }
        }
    }

    Node ancestor(0, { 0,0 });
    table[0][0].prev = &ancestor;
    bigTable[0][0].prev = &ancestor;

    getShortest(bigTable);
    const unsigned int shortest_first = bigTable[table.size() - 1][table.front().size() - 1].cost - bigTable[0][0].cost;
    const unsigned int shortest_second = bigTable.back().back().cost - bigTable[0][0].cost;
    
    std::cout << "(Part 1) Shortest path (small): " << shortest_first << std::endl;
    std::cout << "(Part 2) Shortest path (large): " << shortest_second;
    
    return shortest_first == 393 && shortest_second == 2823 ? 0 : -1;
}
