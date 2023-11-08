#include <iostream>
#include <ctime>

int main() {
    int count = 0,
        bound = 0;

    std::cin >> count >> bound;
    std::srand(std::time(nullptr));

    for (int i = 0; i < count; ++i) {
        int cmd = std::rand() % 2;
        if (cmd) {
            std::cout << "k " << std::rand() % bound << " ";
        }
        else {
            int key1 = std::rand() % bound;
            int key2 = std::rand() % bound;
            std::cout << "q " << std::min(key1, key2) << " " << std::max(key1, key2) << " ";
        }
    }
}